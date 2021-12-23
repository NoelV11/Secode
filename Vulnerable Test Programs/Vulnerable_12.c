Sensitive Data Challenege

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "ScwService.h"
#include "Database.h"
#include "LDAPData.h"
#include "LDAPService.h"
#include "Utils.h"
#include "XMLReader.h"
#include "XMLWriter.h"


const char* ScwService::configurationFileName = "./ScwService.cfg";

ScwService::ScwService() : ldap( NULL ), db( NULL )
{
	superuser = Account("Administrator", "admin", "");
}

bool ScwService::init()
{
	set.clear();
	std::fstream file;
	file.open( configurationFileName, std::ios::in ); // to read
	if( !file.good() )
	{
		std::cout << "Impossible to open the file: " << configurationFileName << std::endl;
		file.close();
		return false;
	}
	// Retrieve every configuration parameter.
	std::string line;
	while( getline( file, line ) )
	{
		if( line.empty() )
		{
			continue;
		}
		set.insert( line );
	}
	file.close();

	// Ask the database password.
	std::cout << "Please enter the database password:" << std::endl;
	std::string password = Utils::getConsoleText();
	set.insert( "db_password=" + password );

	return true;
}

void upperCase( std::string& s )
{
	std::transform( s.begin(), s.end(), s.begin(), ::toupper );
}

ParameterMap ScwService::getParameters( std::string prefix )
{
	ParameterMap map;
	upperCase( prefix );
	for( std::set< std::string >::iterator it = set.begin(); it != set.end(); it++ )
	{
		std::string s = *it;
		upperCase( s );
		// Search an appropriate record.
		size_t pos = s.find( prefix );
		if( pos == std::string::npos )
		{
			continue;
		}
		// Erase the prefix.
		std::string record = *it;
		pos += prefix.length();
		record = record.substr( pos, record.length() - pos );
		// Retrieve key and value.
		pos = record.find('=');
		if( pos == std::string::npos )
		{
			continue;
		}
		std::string key = record.substr( 0, pos );
		std::string value = record.substr( pos + 1, record.length() - ( pos + 1 ) );
		// Keep the result pair.
		map.insert( Parameter( key, value ) );
	}
	return map;
}

void showUsage()
{
	std::cout << "Usage: ScwService [key] [value]" << std::endl;
	std::cout << "\t\t -c (creates/recreates and populates the database.)" << std::endl;
	std::cout << "\t\t -u (specifies a user login for processing.)" << std::endl;
	std::cout << "\t\t -p (specifies a user password for processing.)" << std::endl;
	std::cout << "\t\t -n (changes a user password.)" << std::endl;
	std::cout << "\t\t -a (adds a user record into database.)" << std::endl;
	std::cout << "\t\t -i (imports a list of users from XML file into database.)" << std::endl;
	std::cout << "\t\t -e (exports a list of users into XML file from database.)" << std::endl;
	std::cout << "\t\t -r (repairs a list of users with encrypted passwords.)" << std::endl;
	exit(0);
}

bool ScwService::isNormalMode( int argc, char* argv[] )
{
	for( int i = 1; i < argc; i++ )
	{
		std::string key = argv[ i ];
		if( key == "-c" )
		{
			return false;
		}
	}
	return true;
}

void ScwService::processingSingleKey( std::string key )
{
	if( key == "-n" )
	{
		std::cout << "Please enter your user login:" << std::endl;
		std::string login = Utils::getConsoleText();

		std::cout << "Please enter the current password:" << std::endl;
		std::string currentPassword = Utils::getConsoleText();

		Utils::validPasswordReminder();
		std::cout << "Please enter the new password:" << std::endl;
		std::string newPassword = Utils::getConsoleText();
		if( Utils::isValidPassword( newPassword ) == false )
		{
			std::cout << "The password is not valid. The operation was not completed." << std::endl;
			return;
		}

		// Check the user in database.
		Account account = db->getUser( login, currentPassword );
		if( account.isEmpty() )
		{
			std::cout << "Invalid login or password !" << std::endl;
			return;
		}
		std::cout << "Authorization is all right." << std::endl;

		// Change user password.
		db->changeUserPassword( &account, newPassword );
		return;
	}
	else if( key == "-a" )
	{
		if( isSuperUserVerified() == false )
		{
			return;
		}
		// Form the user credentials.
		std::cout << "Please enter the user name:" << std::endl;
		std::string userName = Utils::getConsoleText();

		Utils::validLoginReminder();
		std::cout << "Please enter the user login:" << std::endl;
		std::string userLogin = Utils::getConsoleText();
		if( Utils::isValidLogin( userLogin ) == false )
		{
			std::cout << "The login is not valid. The operation was not completed." << std::endl;
			return;
		}

		Utils::validPasswordReminder();
		std::cout << "Please enter the user password:" << std::endl;
		std::string userPassword = Utils::getConsoleText();
		if( Utils::isValidPassword( userPassword ) == false )
		{
			std::cout << "The password is not valid. The operation was not completed." << std::endl;
			return;
		}

		// Add user record into database.
		Account account( userName, userLogin, userPassword );
		db->usersAdd( &account );
		return;
	}
	else if( key == "-c" )
	{
		std::cout << "Please enter the super-user login:" << std::endl;
		std::string userLogin = Utils::getConsoleText();
		std::cout << "Please enter the super-user password:" << std::endl;
		std::string userPassword = Utils::getConsoleText();

		LDAPData suData( ldap );
		if( ldap->isAdmin( &suData, userLogin, userPassword ) == false )
		{
			std::cout << "Invalid login or password !" << std::endl;
			return;
		}
		std::cout << "Super-user authorization is all right." << std::endl;
		suData.verify();

		// Add super-user record into database.
		superuser.password = userPassword;
		db->usersAdd( &superuser );

		// Retrieve appropriate data according to a request.
		LDAPData data( ldap );
		ldap->setUsers( &data, "*" );
		data.verify();

		// LDAP and DB data migration.
		data.import( db );
		return;
	}
	showUsage();
}

void ScwService::processing( int argc, char* argv[] )
{
	if( argc == 2 )
	{
		processingSingleKey( argv[ 1 ] );
		return;
	}
	else if( argc % 2 == 0 )
	{
		showUsage();
	}
	// Consider all keys and values.
	const char* login = NULL;
	const char* password = NULL;
	const char* importXmlfile = NULL;
	const char* exportXmlfile = NULL;
	const char* repairXmlfile = NULL;
	for( int i = 1; i < argc; i += 2 )
	{
		std::string key = argv[ i ];
		const char* value = argv[ i + 1 ];
		if( key == "-u" )
		{
			login = value;
		}
		else if( key == "-p" )
		{
			password = value;
		}
		else if( key == "-i" )
		{
			importXmlfile = value;
		}
		else if( key == "-e" )
		{
			exportXmlfile = value;
		}
		else if( key == "-r" )
		{
			repairXmlfile = value;
		}
		else
		{
			showUsage();
		}
	}

	// Consider all possible combinations.
	if( login && password == NULL )
	{
		std::string request = U(login);
		// Retrieve appropriate data according to a request.
		Account account = ldap->getUser( request );
		if( account.isEmpty() )
		{
			std::cout << "Invalid login !" << std::endl;
			return;
		}
		std::cout << "Identification is all right." << std::endl;
		return;
	}
	else if( login && password )
	{
		Account account = db->getUser( login, password );
		if( account.isEmpty() )
		{
			std::cout << "Invalid login or password !" << std::endl;
			return;
		}
		std::cout << "Authorization is all right." << std::endl;
		return;
	}
	else if( importXmlfile )
	{
		if( isSuperUserVerified() == false )
		{
			return;
		}
		// Read an incoming XML data and keep it in the database.
		XMLReader xmlReader;
		if( xmlReader.load( importXmlfile, db ) == false )
		{
			std::cout << "The XML file is not valid. The operation was not completed." << std::endl;
			return;
		}
		return;
	}
	else if( exportXmlfile )
	{
		if( isSuperUserVerified() == false )
		{
			return;
		}
		// Read the database data and keep them in outcoming XML file.
		XMLWriter xmlWriter;
		if( xmlWriter.save( exportXmlfile, db ) == false )
		{
			std::cout << "The XML file was not created. The operation was not completed." << std::endl;
			return;
		}
		return;
	}
	else if( repairXmlfile )
	{
		if( isSuperUserVerified() == false )
		{
			return;
		}
		// Update an incoming XML data file with encrypted passwords.
		XMLReader xmlReader;
		if( xmlReader.load( repairXmlfile, NULL ) == false )
		{
			std::cout << "The XML file is not valid. The operation was not completed." << std::endl;
			return;
		}
		// Update all user passwords.
		std::vector< Account >& accounts = xmlReader.accounts;
		for( size_t i = 0; i < accounts.size(); i++ )
		{
			Account* account = &accounts[ i ];
			account->cipher();
		}
		// Keep accounts in the same XML file.
		XMLWriter xmlWriter;
		xmlWriter.accounts = accounts;
		if( xmlWriter.save( repairXmlfile, NULL ) == false )
		{
			std::cout << "The XML file was not created. The operation was not completed." << std::endl;
			return;
		}
		return;
	}
	showUsage();
}

bool ScwService::isSuperUserVerified()
{
	std::cout << "Please enter the '" << superuser.name << "' password:" << std::endl;
	std::string password = Utils::getConsoleText();
	// Check super-user in database.
	Account account = db->getUser( superuser.login, password );
	if( account.isEmpty() )
	{
		std::cout << "Invalid login or password !" << std::endl;
		throw password;
	}
	return true;
}

bool ScwService::isAccessibleTime()
{
	bool isNotWorkingTime = false;
	tm currentTime = Utils::getCurrentTime();
	// Check it is a weekend.
	if( currentTime.tm_wday > 5 )
	{
		isNotWorkingTime = true;
	}
	// Check it is a working time.
	if( currentTime.tm_hour < workStartHour || workFinalHour <= currentTime.tm_hour )
	{
		isNotWorkingTime = true;
	}
	// Check it is a super-user access.
	if( isNotWorkingTime )
	{
		std::cout << "It is a not working time for the system. You must be a system administrator to have an access." << std::endl;
		// Super-user has an unlimited access to the system.
		return isSuperUserVerified();
	}
	return true;
}


Solution

#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>
#include "ScwService.h"
#include "Database.h"
#include "LDAPData.h"
#include "LDAPService.h"
#include "Utils.h"
#include "XMLReader.h"
#include "XMLWriter.h"


const char* ScwService::configurationFileName = "./ScwService.cfg";

ScwService::ScwService() : ldap( NULL ), db( NULL )
{
	superuser = Account("Administrator", "admin", "");
}

bool ScwService::init()
{
	set.clear();
	std::fstream file;
	file.open( configurationFileName, std::ios::in ); // to read
	if( !file.good() )
	{
		std::cout << "Impossible to open the file: " << configurationFileName << std::endl;
		file.close();
		return false;
	}
	// Retrieve every configuration parameter.
	std::string line;
	while( getline( file, line ) )
	{
		if( line.empty() )
		{
			continue;
		}
		set.insert( line );
	}
	file.close();

	// Ask the database password.
	std::cout << "Please enter the database password:" << std::endl;
	std::string password = Utils::getConsoleText();
	set.insert( "db_password=" + password );

	return true;
}

void upperCase( std::string& s )
{
	std::transform( s.begin(), s.end(), s.begin(), ::toupper );
}

ParameterMap ScwService::getParameters( std::string prefix )
{
	ParameterMap map;
	upperCase( prefix );
	for( std::set< std::string >::iterator it = set.begin(); it != set.end(); it++ )
	{
		std::string s = *it;
		upperCase( s );
		// Search an appropriate record.
		size_t pos = s.find( prefix );
		if( pos == std::string::npos )
		{
			continue;
		}
		// Erase the prefix.
		std::string record = *it;
		pos += prefix.length();
		record = record.substr( pos, record.length() - pos );
		// Retrieve key and value.
		pos = record.find('=');
		if( pos == std::string::npos )
		{
			continue;
		}
		std::string key = record.substr( 0, pos );
		std::string value = record.substr( pos + 1, record.length() - ( pos + 1 ) );
		// Keep the result pair.
		map.insert( Parameter( key, value ) );
	}
	return map;
}

void showUsage()
{
	std::cout << "Usage: ScwService [key] [value]" << std::endl;
	std::cout << "\t\t -c (creates/recreates and populates the database.)" << std::endl;
	std::cout << "\t\t -u (specifies a user login for processing.)" << std::endl;
	std::cout << "\t\t -p (specifies a user password for processing.)" << std::endl;
	std::cout << "\t\t -n (changes a user password.)" << std::endl;
	std::cout << "\t\t -a (adds a user record into database.)" << std::endl;
	std::cout << "\t\t -i (imports a list of users from XML file into database.)" << std::endl;
	std::cout << "\t\t -e (exports a list of users into XML file from database.)" << std::endl;
	std::cout << "\t\t -r (repairs a list of users with encrypted passwords.)" << std::endl;
	exit(0);
}

bool ScwService::isNormalMode( int argc, char* argv[] )
{
	for( int i = 1; i < argc; i++ )
	{
		std::string key = argv[ i ];
		if( key == "-c" )
		{
			return false;
		}
	}
	return true;
}

void ScwService::processingSingleKey( std::string key )
{
	if( key == "-n" )
	{
		std::cout << "Please enter your user login:" << std::endl;
		std::string login = Utils::getConsoleText();

		std::cout << "Please enter the current password:" << std::endl;
		std::string currentPassword = Utils::getConsoleText();

		Utils::validPasswordReminder();
		std::cout << "Please enter the new password:" << std::endl;
		std::string newPassword = Utils::getConsoleText();
		if( Utils::isValidPassword( newPassword ) == false )
		{
			std::cout << "The password is not valid. The operation was not completed." << std::endl;
			return;
		}

		// Check the user in database.
		Account account = db->getUser( login, currentPassword );
		if( account.isEmpty() )
		{
			std::cout << "Invalid login or password !" << std::endl;
			return;
		}
		std::cout << "Authorization is all right." << std::endl;

		// Change user password.
		db->changeUserPassword( &account, newPassword );
		return;
	}
	else if( key == "-a" )
	{
		if( isSuperUserVerified() == false )
		{
			return;
		}
		// Form the user credentials.
		std::cout << "Please enter the user name:" << std::endl;
		std::string userName = Utils::getConsoleText();

		Utils::validLoginReminder();
		std::cout << "Please enter the user login:" << std::endl;
		std::string userLogin = Utils::getConsoleText();
		if( Utils::isValidLogin( userLogin ) == false )
		{
			std::cout << "The login is not valid. The operation was not completed." << std::endl;
			return;
		}

		Utils::validPasswordReminder();
		std::cout << "Please enter the user password:" << std::endl;
		std::string userPassword = Utils::getConsoleText();
		if( Utils::isValidPassword( userPassword ) == false )
		{
			std::cout << "The password is not valid. The operation was not completed." << std::endl;
			return;
		}

		// Add user record into database.
		Account account( userName, userLogin, userPassword );
		db->usersAdd( &account );
		return;
	}
	else if( key == "-c" )
	{
		std::cout << "Please enter the super-user login:" << std::endl;
		std::string userLogin = Utils::getConsoleText();
		std::cout << "Please enter the super-user password:" << std::endl;
		std::string userPassword = Utils::getConsoleText();

		LDAPData suData( ldap );
		if( ldap->isAdmin( &suData, userLogin, userPassword ) == false )
		{
			std::cout << "Invalid login or password !" << std::endl;
			return;
		}
		std::cout << "Super-user authorization is all right." << std::endl;
		suData.verify();

		// Add super-user record into database.
		superuser.password = userPassword;
		db->usersAdd( &superuser );

		// Retrieve appropriate data according to a request.
		LDAPData data( ldap );
		ldap->setUsers( &data, "*" );
		data.verify();

		// LDAP and DB data migration.
		data.import( db );
		return;
	}
	showUsage();
}

void ScwService::processing( int argc, char* argv[] )
{
	if( argc == 2 )
	{
		processingSingleKey( argv[ 1 ] );
		return;
	}
	else if( argc % 2 == 0 )
	{
		showUsage();
	}
	// Consider all keys and values.
	const char* login = NULL;
	const char* password = NULL;
	const char* importXmlfile = NULL;
	const char* exportXmlfile = NULL;
	const char* repairXmlfile = NULL;
	for( int i = 1; i < argc; i += 2 )
	{
		std::string key = argv[ i ];
		const char* value = argv[ i + 1 ];
		if( key == "-u" )
		{
			login = value;
		}
		else if( key == "-p" )
		{
			password = value;
		}
		else if( key == "-i" )
		{
			importXmlfile = value;
		}
		else if( key == "-e" )
		{
			exportXmlfile = value;
		}
		else if( key == "-r" )
		{
			repairXmlfile = value;
		}
		else
		{
			showUsage();
		}
	}

	// Consider all possible combinations.
	if( login && password == NULL )
	{
		std::string request = U(login);
		// Retrieve appropriate data according to a request.
		Account account = ldap->getUser( request );
		if( account.isEmpty() )
		{
			std::cout << "Invalid login !" << std::endl;
			return;
		}
		std::cout << "Identification is all right." << std::endl;
		return;
	}
	else if( login && password )
	{
		Account account = db->getUser( login, password );
		if( account.isEmpty() )
		{
			std::cout << "Invalid login or password !" << std::endl;
			return;
		}
		std::cout << "Authorization is all right." << std::endl;
		return;
	}
	else if( importXmlfile )
	{
		if( isSuperUserVerified() == false )
		{
			return;
		}
		// Read an incoming XML data and keep it in the database.
		XMLReader xmlReader;
		if( xmlReader.load( importXmlfile, db ) == false )
		{
			std::cout << "The XML file is not valid. The operation was not completed." << std::endl;
			return;
		}
		return;
	}
	else if( exportXmlfile )
	{
		if( isSuperUserVerified() == false )
		{
			return;
		}
		// Read the database data and keep them in outcoming XML file.
		XMLWriter xmlWriter;
		if( xmlWriter.save( exportXmlfile, db ) == false )
		{
			std::cout << "The XML file was not created. The operation was not completed." << std::endl;
			return;
		}
		return;
	}
	else if( repairXmlfile )
	{
		if( isSuperUserVerified() == false )
		{
			return;
		}
		// Update an incoming XML data file with encrypted passwords.
		XMLReader xmlReader;
		if( xmlReader.load( repairXmlfile, NULL ) == false )
		{
			std::cout << "The XML file is not valid. The operation was not completed." << std::endl;
			return;
		}
		// Update all user passwords.
		std::vector< Account >& accounts = xmlReader.accounts;
		for( size_t i = 0; i < accounts.size(); i++ )
		{
			Account* account = &accounts[ i ];
			account->cipher();
		}
		// Keep accounts in the same XML file.
		XMLWriter xmlWriter;
		xmlWriter.accounts = accounts;
		if( xmlWriter.save( repairXmlfile, NULL ) == false )
		{
			std::cout << "The XML file was not created. The operation was not completed." << std::endl;
			return;
		}
		return;
	}
	showUsage();
}

bool ScwService::isSuperUserVerified()
{
	std::cout << "Please enter the '" << superuser.name << "' password:" << std::endl;
	std::string password = Utils::getConsoleText();
	// Check super-user in database.
	Account account = db->getUser( superuser.login, password );
	if( account.isEmpty() )
	{
		std::cout << "Invalid login or password !" << std::endl;
		return false;
	}
	return true;
}

bool ScwService::isAccessibleTime()
{
	bool isNotWorkingTime = false;
	tm currentTime = Utils::getCurrentTime();
	// Check it is a weekend.
	if( currentTime.tm_wday > 5 )
	{
		isNotWorkingTime = true;
	}
	// Check it is a working time.
	if( currentTime.tm_hour < workStartHour || workFinalHour <= currentTime.tm_hour )
	{
		isNotWorkingTime = true;
	}
	// Check it is a super-user access.
	if( isNotWorkingTime )
	{
		std::cout << "It is a not working time for the system. You must be a system administrator to have an access." << std::endl;
		// Super-user has an unlimited access to the system.
		return isSuperUserVerified();
	}
	return true;
}
