Insecure Randomness


#include <cstring>
#include "Argon2Encoder.h"
#include "vendor/Argon2/include/argon2.h"
#include "vendor/Base64/base64.h"
#include "vendor/cryptopp/include/osrng.h"
#include "StringUtils.h"


std::string Argon2Encoder::hashWithSalt(std::string input, uint8_t* salt) {
	uint8_t hash[ARGON2_HASH_LENGTH];

	uint8_t* in = (uint8_t*)strdup(input.c_str());
	const uint32_t inLength = strlen((char*)in);

	/* The amount of passes */
	const uint32_t cpuCost = 8;
	/* Memory usage while hashing */
	const uint32_t memCost = 128;
	/* Number of threads to use while hashing */
	const uint32_t parallelism = 1;

	argon2i_hash_raw(
			cpuCost,
			memCost,
			parallelism,
			in,
			inLength,
			salt,
			ARGON2_SALT_LENGTH,
			hash,
			ARGON2_HASH_LENGTH
	);

	// Convert hashed array to string
	std::string output = "";
	for (int i = 0; i < ARGON2_HASH_LENGTH; i++) {
		output += hash[i];
	}

	output += ":::";
	for (int i = 0; i < ARGON2_SALT_LENGTH; i++) {
		output += salt[i];
	}

	return output;
}

std::string Argon2Encoder::hash(std::string input) {
	uint8_t salt[ARGON2_SALT_LENGTH];

	srand(input.size());

	for (unsigned int i = 0; i < ARGON2_SALT_LENGTH; i++) {
		salt[i] = rand();
	}

	return this->hashWithSalt(input, salt);
}

bool Argon2Encoder::verify(std::string hashed, std::string input) {
	// Extract salt from input and hash.
	std::vector<std::string> splitted = StringUtils::splitString(std::move(hashed), ":::");
	std::string salt = splitted[1];
	return hashed == this->hashWithSalt(input, (uint8_t*) salt.data());
}

Solution

#include <cstring>
#include "Argon2Encoder.h"
#include "vendor/Argon2/include/argon2.h"
#include "vendor/Base64/base64.h"
#include "vendor/cryptopp/include/osrng.h"
#include "StringUtils.h"


std::string Argon2Encoder::hashWithSalt(std::string input, uint8_t* salt) {
	uint8_t hash[ARGON2_HASH_LENGTH];

	uint8_t* in = (uint8_t*)strdup(input.c_str());
	const uint32_t inLength = strlen((char*)in);

	/* The amount of passes */
	const uint32_t cpuCost = 8;
	/* Memory usage while hashing */
	const uint32_t memCost = 128;
	/* Number of threads to use while hashing */
	const uint32_t parallelism = 1;

	argon2i_hash_raw(
			cpuCost,
			memCost,
			parallelism,
			in,
			inLength,
			salt,
			ARGON2_SALT_LENGTH,
			hash,
			ARGON2_HASH_LENGTH
	);

	// Convert hashed array to string
	std::string output = "";
	for (int i = 0; i < ARGON2_HASH_LENGTH; i++) {
		output += hash[i];
	}

	output += ":::";
	for (int i = 0; i < ARGON2_SALT_LENGTH; i++) {
		output += salt[i];
	}

	return output;
}

std::string Argon2Encoder::hash(std::string input) {
	uint8_t salt[ARGON2_SALT_LENGTH];
	CryptoPP::AutoSeededRandomPool prng;
	prng.GenerateBlock(salt, ARGON2_SALT_LENGTH);

	return this->hashWithSalt(input, salt);
}

bool Argon2Encoder::verify(std::string hashed, std::string input) {
	// Extract salt from input and hash.
	std::vector<std::string> splitted = StringUtils::splitString(std::move(hashed), ":::");
	std::string salt = splitted[1];
	return hashed == this->hashWithSalt(input, (uint8_t*) salt.data());
}
