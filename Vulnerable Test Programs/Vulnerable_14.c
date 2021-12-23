Buffer Overflow

#include <iostream>
#include <string>
#include "LDAPService.h"
#include "ScwService.h"


LDAPService::LDAPService( ScwService* p )
{
	ldap = NULL;
	parent = p;
	parent->ldap = this;
}

void LDAPService::printError( std::string text, unsigned long code )
{
	std::string error = ldap_err2string( code );
	// Display LDAP error using a template.
	char buffer[ 32 ];
	sprintf_s( buffer, "'%s' failed with error: %s", text.c_str(), error.c_str() );
	std::cout << buffer << std::endl;
}

bool LDAPService::isConfigured()
{
	// Retrieve configuration parameters.
	ParameterMap map = parent->getParameters("ldap_");

	std::string connectionRecord;
	for( ParameterMap::iterator it = map.begin(); it != map.end(); it++ )
	{
		std::string key = it->first;
		std::string value = it->second;
		if( key == "host" )
		{
			host = value;
		}
		else if( key == "base" )
		{
			base = value;
		}
	}
	return host.size() && base.size();
}

bool LDAPService::init()
{
	if( isConfigured() == false )
	{
		std::cout << "Error: some LDAP configuration parameters are missing." << std::endl;
		return false;
	}

	ldap = ldap_init( U(host), LDAP_PORT);
	if( ldap == NULL )
	{
		printError("ldap_init", HRESULT_FROM_WIN32( GetLastError() ) );
		return false;
	}
	
	ULONG version = LDAP_VERSION3;
	ULONG result = ldap_set_option( ldap, LDAP_OPT_PROTOCOL_VERSION, &version );
	if( result != LDAP_SUCCESS )
	{
		printError("ldap_set_option", result );
		return false;
	}

	// Connect to the server.
	result = ldap_connect( ldap, NULL );
	if( result != LDAP_SUCCESS )
	{
		printError("ldap_connect", result );
		return false;
	}
	return true;
}

void LDAPService::set( LDAPData* data, std::string request )
{
	data->clear();
	std::string filter = std::string("(&(uid=") + request + "))";

	int result = ldap_search_s( ldap,
								U(base),
								LDAP_SCOPE_SUBTREE,
								U(filter),
								LDAPObject::ldapAttributes,
								false,
								&data->data);
	if( result != LDAP_SUCCESS )
	{
		data->clear();
		printError("ldap_search_s", result );
		return;
	}
	data->get();
}

LDAPService::~LDAPService()
{
	ldap_unbind( ldap );
}


Solutions

#include <iostream>
#include <string>
#include "LDAPService.h"
#include "ScwService.h"


LDAPService::LDAPService( ScwService* p )
{
	ldap = NULL;
	parent = p;
	parent->ldap = this;
}

void LDAPService::printError( std::string text, unsigned long code )
{
	std::string error = ldap_err2string( code );
	// Display LDAP error using a template.
	std::string templ = "'%s' failed with error: %s";
	// Calculate the minimal size for the buffer: taking into account '\0' and replaced symbols for arguments (%).
	size_t size = templ.size() + 1 + text.size() - 2 + error.size() - 2;

	char* buffer = new char[ size ];
	sprintf_s( buffer, size, templ.c_str(), text.c_str(), error.c_str() );
	std::cout << buffer << std::endl;
	delete[] buffer;
}

bool LDAPService::isConfigured()
{
	// Retrieve configuration parameters.
	ParameterMap map = parent->getParameters("ldap_");

	std::string connectionRecord;
	for( ParameterMap::iterator it = map.begin(); it != map.end(); it++ )
	{
		std::string key = it->first;
		std::string value = it->second;
		if( key == "host" )
		{
			host = value;
		}
		else if( key == "base" )
		{
			base = value;
		}
	}
	return host.size() && base.size();
}

bool LDAPService::init()
{
	if( isConfigured() == false )
	{
		std::cout << "Error: some LDAP configuration parameters are missing." << std::endl;
		return false;
	}

	ldap = ldap_init( U(host), LDAP_PORT);
	if( ldap == NULL )
	{
		printError("ldap_init", HRESULT_FROM_WIN32( GetLastError() ) );
		return false;
	}
	
	ULONG version = LDAP_VERSION3;
	ULONG result = ldap_set_option( ldap, LDAP_OPT_PROTOCOL_VERSION, &version );
	if( result != LDAP_SUCCESS )
	{
		printError("ldap_set_option", result );
		return false;
	}

	// Connect to the server.
	result = ldap_connect( ldap, NULL );
	if( result != LDAP_SUCCESS )
	{
		printError("ldap_connect", result );
		return false;
	}
	return true;
}

void LDAPService::set( LDAPData* data, std::string request )
{
	data->clear();
	std::string filter = std::string("(&(uid=") + request + "))";

	int result = ldap_search_s( ldap,
								U(base),
								LDAP_SCOPE_SUBTREE,
								U(filter),
								LDAPObject::ldapAttributes,
								false,
								&data->data);
	if( result != LDAP_SUCCESS )
	{
		data->clear();
		printError("ldap_search_s", result );
		return;
	}
	data->get();
}

LDAPService::~LDAPService()
{
	ldap_unbind( ldap );
}
