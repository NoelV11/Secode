Insecure Randmness


#include "AESCryptor.h"

#include "vendor/cryptopp/include/aes.h"
#include "vendor/cryptopp/include/modes.h"
#include "vendor/cryptopp/include/filters.h"
#include "vendor/cryptopp/include/osrng.h"
#include "vendor/Base64/base64.h"
#include "StringUtils.h"
#include "vendor/cryptopp/include/hkdf.h"
#include "vendor/cryptopp/include/gcm.h"
#include "Utils.h"
#include "Argon2Encoder.h"

std::string AESCryptor::getIV(const std::string cipherText) const {
	auto splitted = StringUtils::splitString(cipherText, ":::");
	return splitted[1];
}

std::string AESCryptor::getCipherText(const std::string cipherText) const {
	auto splitted = StringUtils::splitString(cipherText, ":::");
	return splitted[0];
}

CryptoPP::SecByteBlock AESCryptor::generateKey(
	User& user,
	CryptoPP::SecByteBlock iv
) const {
	CryptoPP::SecByteBlock generatedKey(CryptoPP::AES::MAX_KEYLENGTH);

	// Key derivation function 
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
	hkdf.DeriveKey(
		generatedKey,
		generatedKey.size(),
		(const CryptoPP::byte*) user.getPassword().data(),
		user.getPassword().size(),
		(const CryptoPP::byte*) iv.data(),
		iv.size(),
		nullptr,
		0
	);

	return generatedKey;
}

std::string AESCryptor::encrypt(std::string input, User& user) {
	try {
		CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);

#ifdef _WIN32
		std::string hostname = getenv("COMPUTERNAME");
#else
		std::string hostname = getenv("HOSTNAME");
#endif

		Argon2Encoder encoder;
		std::string hashedHostname = encoder.hash(hostname);
		for (unsigned int i = 0; i < iv.size(); i++)
		{
			iv[i] = hashedHostname[i % hashedHostname.size()];
		}

		CryptoPP::SecByteBlock key = this->generateKey(user, iv);

		std::string cipherText;

		CryptoPP::GCM<CryptoPP::AES>::Encryption aesEncryption;
		aesEncryption.SetKeyWithIV(
			(const CryptoPP::byte*)key.data(),
			key.size(),
			(const CryptoPP::byte*)iv.data(),
			iv.size()
		);

		CryptoPP::AuthenticatedEncryptionFilter streamFilter(
			aesEncryption,
			new CryptoPP::StringSink(cipherText)
		);
		streamFilter.Put(
			reinterpret_cast<const unsigned char*>(input.c_str()),
			input.length() + 1
		);
		streamFilter.MessageEnd();

		cipherText += ":::" + std::string(
			reinterpret_cast<const char*>(iv.data()),
			iv.size()
		);

		std::string encodedCipherText;
		Base64::Encode(cipherText, &encodedCipherText);

		return encodedCipherText;
	} catch (CryptoPP::Exception e) {
		Utils::displayError("Encryption failed.");
		Utils::niceExit(3);
		return "";
	}
}

std::string AESCryptor::decrypt(std::string input, User& user) {
	try {
		std::string decodedText;
		Base64::Decode(input, &decodedText);

		std::string ivString = this->getIV(decodedText);

		CryptoPP::SecByteBlock iv(
			reinterpret_cast<const CryptoPP::byte*>(ivString.data()),
			ivString.size()
		);
		CryptoPP::SecByteBlock key = this->generateKey(user, iv);

		input = this->getCipherText(decodedText);

		CryptoPP::GCM<CryptoPP::AES>::Decryption aesDecryption;
		aesDecryption.SetKeyWithIV(
			(const CryptoPP::byte*)key.data(),
			key.size(),
			(const CryptoPP::byte*)iv.data(),
			iv.size()
		);

		std::string decryptedText;

		CryptoPP::AuthenticatedDecryptionFilter streamFilter(
			aesDecryption,
			new CryptoPP::StringSink(decryptedText)
		);
		streamFilter.Put(reinterpret_cast<const unsigned char*>(
			input.c_str()),
			input.size()
		);
		streamFilter.MessageEnd();

		return decryptedText;
	} catch (CryptoPP::Exception e) {
		Utils::displayError("Decryption failed.");
		Utils::niceExit(4);
		return "";
	}
}


Solution

#include "AESCryptor.h"

#include "vendor/cryptopp/include/aes.h"
#include "vendor/cryptopp/include/modes.h"
#include "vendor/cryptopp/include/filters.h"
#include "vendor/cryptopp/include/osrng.h"
#include "vendor/Base64/base64.h"
#include "StringUtils.h"
#include "vendor/cryptopp/include/hkdf.h"
#include "vendor/cryptopp/include/gcm.h"
#include "Utils.h"

std::string AESCryptor::getIV(const std::string cipherText) const {
	auto splitted = StringUtils::splitString(cipherText, ":::");
	return splitted[1];
}

std::string AESCryptor::getCipherText(const std::string cipherText) const {
	auto splitted = StringUtils::splitString(cipherText, ":::");
	return splitted[0];
}

CryptoPP::SecByteBlock AESCryptor::generateKey(
	User& user,
	CryptoPP::SecByteBlock iv
) const {
	CryptoPP::SecByteBlock generatedKey(CryptoPP::AES::MAX_KEYLENGTH);

	// Key derivation function 
	CryptoPP::HKDF<CryptoPP::SHA256> hkdf;
	hkdf.DeriveKey(
		generatedKey,
		generatedKey.size(),
		(const CryptoPP::byte*) user.getPassword().data(),
		user.getPassword().size(),
		(const CryptoPP::byte*) iv.data(),
		iv.size(),
		nullptr,
		0
	);

	return generatedKey;
}

std::string AESCryptor::encrypt(std::string input, User& user) {
	try {
		CryptoPP::AutoSeededRandomPool prng;

		CryptoPP::SecByteBlock iv(CryptoPP::AES::BLOCKSIZE);

		prng.GenerateBlock(iv, iv.size());
		CryptoPP::SecByteBlock key = this->generateKey(user, iv);

		std::string cipherText;

		CryptoPP::GCM<CryptoPP::AES>::Encryption aesEncryption;
		aesEncryption.SetKeyWithIV(
			(const CryptoPP::byte*)key.data(),
			key.size(),
			(const CryptoPP::byte*)iv.data(),
			iv.size()
		);

		CryptoPP::AuthenticatedEncryptionFilter streamFilter(
			aesEncryption,
			new CryptoPP::StringSink(cipherText)
		);
		streamFilter.Put(
			reinterpret_cast<const unsigned char*>(input.c_str()),
			input.length() + 1
		);
		streamFilter.MessageEnd();

		cipherText += ":::" + std::string(
			reinterpret_cast<const char*>(iv.data()),
			iv.size()
		);

		std::string encodedCipherText;
		Base64::Encode(cipherText, &encodedCipherText);

		return encodedCipherText;
	} catch (CryptoPP::Exception e) {
		Utils::displayError("Encryption failed.");
		Utils::niceExit(3);
		return "";
	}
}

std::string AESCryptor::decrypt(std::string input, User& user) {
	try {
		std::string decodedText;
		Base64::Decode(input, &decodedText);

		std::string ivString = this->getIV(decodedText);

		CryptoPP::SecByteBlock iv(
			reinterpret_cast<const CryptoPP::byte*>(ivString.data()),
			ivString.size()
		);
		CryptoPP::SecByteBlock key = this->generateKey(user, iv);

		input = this->getCipherText(decodedText);

		CryptoPP::GCM<CryptoPP::AES>::Decryption aesDecryption;
		aesDecryption.SetKeyWithIV(
			(const CryptoPP::byte*)key.data(),
			key.size(),
			(const CryptoPP::byte*)iv.data(),
			iv.size()
		);

		std::string decryptedText;

		CryptoPP::AuthenticatedDecryptionFilter streamFilter(
			aesDecryption,
			new CryptoPP::StringSink(decryptedText)
		);
		streamFilter.Put(reinterpret_cast<const unsigned char*>(
			input.c_str()),
			input.size()
		);
		streamFilter.MessageEnd();

		return decryptedText;
	} catch (CryptoPP::Exception e) {
		Utils::displayError("Decryption failed.");
		Utils::niceExit(4);
		return "";
	}
}
