Challenege - Buffer Overflow
#include <algorithm>
#include <io.h>
#include <iostream>
#include "Utils.h"


bool isValid( std::string record, char symbols[], int number )
{
	for( int i = 0; i < record.length(); i++ )
	{
		char symbol = record[ i ];
		for( int k = 0; k < number; k++ )
		{
			if( symbols[ k ] == symbol )
			{
				return false;
			}
		}
	}
	return true;
}

bool Utils::isValidFilter( std::string filter )
{
	char symbols[] = {'\\', '*', '(', ')', '\0', '/', '&', '!', '|', ',', '-', '+', '~', '<', '>', ';', '"', '\'', '=', '#'};
	return isValid( filter, symbols, sizeof(symbols) / sizeof(char) );
}

bool Utils::isNotCommand( std::string parameter )
{
	char symbols[] = {'&', '|', ';', '<', '>'};
	return isValid( parameter, symbols, sizeof(symbols) / sizeof(char) );
}

std::string Utils::getConsoleText()
{
	const int size = 8;
	char buf[ size ];
	gets( buf );
	return buf;
}

Solution

#include <algorithm>
#include <io.h>
#include <iostream>
#include "Utils.h"


bool isValid( std::string record, char symbols[], int number )
{
	for( int i = 0; i < record.length(); i++ )
	{
		char symbol = record[ i ];
		for( int k = 0; k < number; k++ )
		{
			if( symbols[ k ] == symbol )
			{
				return false;
			}
		}
	}
	return true;
}

bool Utils::isValidFilter( std::string filter )
{
	char symbols[] = {'\\', '*', '(', ')', '\0', '/', '&', '!', '|', ',', '-', '+', '~', '<', '>', ';', '"', '\'', '=', '#'};
	return isValid( filter, symbols, sizeof(symbols) / sizeof(char) );
}

bool Utils::isNotCommand( std::string parameter )
{
	char symbols[] = {'&', '|', ';', '<', '>'};
	return isValid( parameter, symbols, sizeof(symbols) / sizeof(char) );
}

std::string Utils::getConsoleText()
{
	std::string text;
	std::cin >> text;
	return text;
}
