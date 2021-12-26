Plaintext Password Storage

#include <iostream>
#include "Account.h"
#include "Database.h"
#include "DBQuery.h"
#include "ScwService.h"
#include "Utils.h"

Account Database::getUser( std::string login, std::string password )
{
	Account account;
	DBQuery dbQuery("SELECT name, login, password, salt, "
                    "EXTRACT(EPOCH FROM expiration), "
                    "EXTRACT(EPOCH FROM authentification), "
                    "attempts FROM users WHERE login = $1::varchar");
	dbQuery.arg( login );
	PGresult* result = dbQuery.execute( connection );

	if( PQresultStatus( result ) != PGRES_TUPLES_OK )
	{
		error( dbQuery.get(), result );
		return account;
	}
	int number = PQntuples( result );
	for( int i = 0; i < number; i++ )
	{
		account.update( result, i );
	}
	PQclear( result );

	// Verify the login is valid.
	if( account.isEmpty() )
	{
		return account;
	}
	// Verify the ability of authentification.
	if( account.attempts >= Account::maximalAttempts )
	{
		std::cout << "Error: multiple invalid attempts of "
                     "authentification with login '" << account.login
                  << "'." << std::endl;
		std::cout << "\t Previous attempt time: "
                  << Utils::getTime( account.authentification ) << std::endl;
		std::cout << "\t The account is blocked. "
                     "Please try to contact an administrator." << std::endl;
		return Account();
	}
	// Verify the password is valid.
	if( account.password == scrambler.cipher( password, account.salt ) )
	{
		updatePasswordData( login, "0" );

		// Check the password expiration status.
		time_t currentTime = Utils::getCurrentTimestamp();
		if( account.expiration + Account::expirationShift < currentTime )
		{
			std::cout << "The password has expired." << std::endl;
			Utils::validPasswordReminder();
			std::cout << "Please enter the new password:" << std::endl;
			std::string newPassword = Utils::getConsoleText();
			if( Utils::isValidPassword( newPassword ) == false )
			{
				std::cout << "The password is not valid. "
                             "The operation was not completed." << std::endl;
				return Account();
			}
			// Change user password.
			changeUserPassword( &account, newPassword );
		}
		return account;
	}
	// The password is wrong.
	updatePasswordData( login, Utils::value2string( account.attempts + 1 ) );
	return Account();
}

void Database::changeUserPassword( Account* account, std::string password )
{
	account->password = password;
	
	DBQuery dbQuery("UPDATE users SET password = $2::varchar, "
                    "salt = $3::varchar, expiration = current_timestamp "
                    "WHERE login = $1::varchar");
	dbQuery.arg( account->login ).arg( account->password ).arg( account->salt );
	PGresult* result = dbQuery.execute( connection );
	if( PQresultStatus( result ) != PGRES_COMMAND_OK )
	{
		error( dbQuery.get(), result );
		return;
	}
	PQclear( result );
}


Solution

#include <iostream>
#include "Account.h"
#include "Database.h"
#include "DBQuery.h"
#include "ScwService.h"
#include "Utils.h"

Account Database::getUser( std::string login, std::string password )
{
	Account account;
	DBQuery dbQuery("SELECT name, login, password, salt, "
                    "EXTRACT(EPOCH FROM expiration), "
                    "EXTRACT(EPOCH FROM authentification), "
                    "attempts FROM users WHERE login = $1::varchar");
	dbQuery.arg( login );
	PGresult* result = dbQuery.execute( connection );

	if( PQresultStatus( result ) != PGRES_TUPLES_OK )
	{
		error( dbQuery.get(), result );
		return account;
	}
	int number = PQntuples( result );
	for( int i = 0; i < number; i++ )
	{
		account.update( result, i );
	}
	PQclear( result );

	// Verify the login is valid.
	if( account.isEmpty() )
	{
		return account;
	}
	// Verify the ability of authentification.
	if( account.attempts >= Account::maximalAttempts )
	{
		std::cout << "Error: multiple invalid attempts of "
                     "authentification with login '" << account.login
                  << "'." << std::endl;
		std::cout << "\t Previous attempt time: "
                  << Utils::getTime( account.authentification ) << std::endl;
		std::cout << "\t The account is blocked. "
                     "Please try to contact an administrator." << std::endl;
		return Account();
	}
	// Verify the password is valid.
	if( account.password == scrambler.cipher( password, account.salt ) )
	{
		updatePasswordData( login, "0" );

		// Check the password expiration status.
		time_t currentTime = Utils::getCurrentTimestamp();
		if( account.expiration + Account::expirationShift < currentTime )
		{
			std::cout << "The password has expired." << std::endl;
			Utils::validPasswordReminder();
			std::cout << "Please enter the new password:" << std::endl;
			std::string newPassword = Utils::getConsoleText();
			if( Utils::isValidPassword( newPassword ) == false )
			{
				std::cout << "The password is not valid. "
                             "The operation was not completed." << std::endl;
				return Account();
			}
			// Change user password.
			changeUserPassword( &account, newPassword );
		}
		return account;
	}
	// The password is wrong.
	updatePasswordData( login, Utils::value2string( account.attempts + 1 ) );
	return Account();
}

void Database::changeUserPassword( Account* account, std::string password )
{
	account->password = password;
	account->cipher();
	DBQuery dbQuery("UPDATE users SET password = $2::varchar, "
                    "salt = $3::varchar, expiration = current_timestamp "
                    "WHERE login = $1::varchar");
	dbQuery.arg( account->login ).arg( account->password ).arg( account->salt );
	PGresult* result = dbQuery.execute( connection );
	if( PQresultStatus( result ) != PGRES_COMMAND_OK )
	{
		error( dbQuery.get(), result );
		return;
	}
	PQclear( result );
}
