Logical Error

#include "stdafx.h"


#include "TlsServer.h"


#include "Macro.h"


#include <cstdlib>


#include <iostream>


bool TlsServer::GenerateKey() throw()
{
    const int result = system("openssl genrsa "
                              "-out " PKEY_FNAME_ " "
                              "-aes256 " // AES CBC
                              "-f4 " MAKE_STR_(RSA_BITS_));

    return (0 == result) ? true : false;
}

// Uses existing key
bool TlsServer::GenerateCertificate() throw()
{
    /*
    /C=	 Country
    /ST= State
    /L=  Location
    /O=  Organization
    /OU= Organizational Unit
    /CN= Common Name
    */
    const int result =
        system("openssl req "
               "-config openssl.cnf "
               "-x509 -new -out " CERT_FNAME_ " "
               "-key " PKEY_FNAME_ " "
               "-days 1 -subj /C=RU/ST=Ul/L=Ul/O=SS/OU=IT/CN=Server "
               "-sha3-512"); // openssl list -digest-commands

    return (0 == result) ? true : false;
}

// 2048 bits: less than a minute
// 4096 bits: less than half of an hour
// 8192 bits: less than a day
bool TlsServer::GenerateDHParams() throw()
{
    const int result = system("openssl dhparam "
                              "-outform PEM -out " DH_FNAME_ " "
                              "-check " MAKE_STR_(RSA_BITS_));

    return (0 == result) ? true : false;
}

bool TlsServer::GenerateArtifacts() throw()
{
    bool certOk = false;

    const bool keyOk = TlsServer::GenerateKey();
    std::cout << std::endl << std::endl;

    if (keyOk)
    {
        certOk = TlsServer::GenerateCertificate();
        std::cout << std::endl << std::endl;
    }
    const bool paramsOk = TlsServer::GenerateDHParams();
    std::cout << std::endl << std::endl;

    return certOk || keyOk || paramsOk;
}

Solution

#include "stdafx.h"


#include "TlsServer.h"


#include "Macro.h"


#include <cstdlib>


#include <iostream>


bool TlsServer::GenerateKey() throw()
{
    const int result = system("openssl genrsa "
                              "-out " PKEY_FNAME_ " "
                              "-aes256 " // AES CBC
                              "-f4 " MAKE_STR_(RSA_BITS_));

    return (0 == result) ? true : false;
}

// Uses existing key
bool TlsServer::GenerateCertificate() throw()
{
    /*
    /C=	 Country
    /ST= State
    /L=  Location
    /O=  Organization
    /OU= Organizational Unit
    /CN= Common Name
    */
    const int result =
        system("openssl req "
               "-config openssl.cnf "
               "-x509 -new -out " CERT_FNAME_ " "
               "-key " PKEY_FNAME_ " "
               "-days 1 -subj /C=RU/ST=Ul/L=Ul/O=SS/OU=IT/CN=Server "
               "-sha3-512"); // openssl list -digest-commands

    return (0 == result) ? true : false;
}

// 2048 bits: less than a minute
// 4096 bits: less than half of an hour
// 8192 bits: less than a day
bool TlsServer::GenerateDHParams() throw()
{
    const int result = system("openssl dhparam "
                              "-outform PEM -out " DH_FNAME_ " "
                              "-check " MAKE_STR_(RSA_BITS_));

    return (0 == result) ? true : false;
}

bool TlsServer::GenerateArtifacts() throw()
{
    bool certOk = false;

    const bool keyOk = TlsServer::GenerateKey();
    std::cout << std::endl << std::endl;

    if (keyOk)
    {
        certOk = TlsServer::GenerateCertificate();
        std::cout << std::endl << std::endl;
    }
    const bool paramsOk = TlsServer::GenerateDHParams();
    std::cout << std::endl << std::endl;

    return certOk && keyOk && paramsOk;
}


const std::string TlsServer::CertFileName       = CERT_FNAME_;
const std::string TlsServer::PrivateKeyFileName = PKEY_FNAME_;
const std::string TlsServer::DHFileName         = DH_FNAME_;
