#include <SQLiteCpp/Statement.h>
#include <iostream>

#include "Database.h"
#include "vendor/Base64/base64.h"
#include "Utils.h"


void Database::checkDatabase() {
	db = new SQLite::Database(
		/* DB path */ db_filename,
		/* DB flags */ SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE
	);
}

void Database::displayUnrecoverableError() const {
	Utils::displayError("An unexpected exception occurred!");
	Utils::niceExit(2);
}

Database::Database(std::string& db_filename, User& user):
    db_filename(db_filename),
    user(user)
{}

Database::~Database() {
	delete this->db;
}

bool Database::exists() {
	checkDatabase();

	try {
		// Test if this is a new database
		if (!db->tableExists(this->USERS_TABLE)) {
			return false;
		}

		// Test if the database currently has an owner
		SQLite::Statement query(
			/* Database object */ *db,
			/* SQL Query */
			"SELECT username FROM " + this->USERS_TABLE
		);
		return query.executeStep();
	} catch (SQLite::Exception e) {
		this->displayUnrecoverableError();
		return false;
	}
}

void Database::initialize() {
	checkDatabase();

	try {
		db->exec("DROP TABLE IF EXISTS " + this->USERS_TABLE);
		db->exec("DROP TABLE IF EXISTS " + this->SECRETS_TABLE);

		// Add USER table in case of new database
		db->exec(
			"CREATE TABLE " + this->USERS_TABLE + " (" +
			"username  TEXT," +
			"password TEXT" +
			");"
		);

		SQLite::Statement query(
			/* Database object */ *db,
			/* SQL Query */
			"INSERT INTO " + this->USERS_TABLE + " (username, password)"
			"VALUES (?, ?);"
		);

		std::string encodedPassword;
		const std::string password = this->user.getPassword();
		Base64::Encode(password, &encodedPassword);

		query.bind(1, this->user.getUsername());
		query.bind(2, encodedPassword);

		query.exec();

		db->exec(
			"CREATE TABLE " + this->SECRETS_TABLE + " ("
			"NAME TEXT, " +
			"content TEXT" +
			")"
		);
	} catch (SQLite::Exception e) {
		this->displayUnrecoverableError();
	}
}

bool Database::checkCredentials() {
	checkDatabase();

	try {
		// Test if this is a new database
		if (!db->tableExists(this->USERS_TABLE)) {
			return false;
		}

		// Test if the database currently has an owner
		SQLite::Statement query(
			/* Database object */ *db,
			/* SQL Query */
			"SELECT username, password FROM " + this->USERS_TABLE
		);

		if (!query.executeStep()) {
			return false;
		}

		std::string username = query.getColumn(0).getText();
		const std::string hashedPassword = query.getColumn(1).getText();
		std::string decoded;
		Base64::Decode(hashedPassword, &decoded);

		return decoded == this->user.getPassword();
	} catch (SQLite::Exception e) {
		this->displayUnrecoverableError();
		return false;
	}
}

void Database::writeSecret(const std::string name, const std::string secret) {
	checkDatabase();

	try {
		SQLite::Statement query(
			/* Database object */ *db,
			/* SQL Query */
			"INSERT INTO " + this->SECRETS_TABLE + " " +
			"(name, content) VALUES (?, ?)"
		);
		query.bind(1, name);
		query.bind(2, secret);

		query.exec();
	} catch (SQLite::Exception e) {
		Utils::displayError(
			"Could not write secret with name " + name + "!"
		);
	}
}

std::string Database::readSecret(const std::string name) {
	checkDatabase();

	try {
		SQLite::Statement query(
			/* Database object */ *db,
			/* SQL Query */
			"SELECT content FROM " + this->SECRETS_TABLE + " WHERE name = ?;"
		);
		query.bind(1, name);

		if (!query.executeStep()) {
			return "";
		}

		return query.getColumn(0).getText();
	} catch (SQLite::Exception e) {
		Utils::displayError(
			"Could not read secret " + name + " from database!"
		);
		return "";
	}
}

User& Database::getUser() const {
	return this->user;
}

Solution


#include <SQLiteCpp/Statement.h>
#include <iostream>

#include "Database.h"
#include "vendor/Base64/base64.h"
#include "Utils.h"

void Database::displayUnrecoverableError() const {
	Utils::displayError("An unexpected exception occurred!");
	Utils::niceExit(2);
}

Database::Database(std::string& db_filename, User& user):
    db_filename(db_filename),
    user(user)
{
	this->db = std::make_unique<SQLite::Database>(
			/* DB path */ db_filename,
			/* DB flags */ SQLite::OPEN_CREATE | SQLite::OPEN_READWRITE
	);
}

Database::~Database() {}

bool Database::exists() {
	try {
		// Test if this is a new database
		if (!db->tableExists(this->USERS_TABLE)) {
			return false;
		}

		// Test if the database currently has an owner
		SQLite::Statement query(
			/* Database object */ *(db.get()),
			/* SQL Query */
			"SELECT username FROM " + this->USERS_TABLE
		);
		return query.executeStep();
	} catch (SQLite::Exception e) {
		this->displayUnrecoverableError();
		return false;
	}
}

void Database::initialize() {
	try {
		db->exec("DROP TABLE IF EXISTS " + this->USERS_TABLE);
		db->exec("DROP TABLE IF EXISTS " + this->SECRETS_TABLE);

		// Add USER table in case of new database
		db->exec(
			"CREATE TABLE " + this->USERS_TABLE + " (" +
			"username  TEXT," +
			"password TEXT" +
			");"
		);

		SQLite::Statement query(
			/* Database object */ *(db.get()),
			/* SQL Query */
			"INSERT INTO " + this->USERS_TABLE + " (username, password)"
			"VALUES (?, ?);"
		);

		std::string encodedPassword;
		const std::string password = this->user.getPassword();
		Base64::Encode(password, &encodedPassword);

		query.bind(1, this->user.getUsername());
		query.bind(2, encodedPassword);

		query.exec();

		db->exec(
			"CREATE TABLE " + this->SECRETS_TABLE + " ("
			"NAME TEXT, " +
			"content TEXT" +
			")"
		);
	} catch (SQLite::Exception e) {
		this->displayUnrecoverableError();
	}
}

bool Database::checkCredentials() {
	try {
		// Test if this is a new database
		if (!db->tableExists(this->USERS_TABLE)) {
			return false;
		}

		// Test if the database currently has an owner
		SQLite::Statement query(
			/* Database object */ *(db.get()),
			/* SQL Query */
			"SELECT username, password FROM " + this->USERS_TABLE
		);

		if (!query.executeStep()) {
			return false;
		}

		std::string username = query.getColumn(0).getText();
		const std::string hashedPassword = query.getColumn(1).getText();
		std::string decoded;
		Base64::Decode(hashedPassword, &decoded);

		return decoded == this->user.getPassword();
	} catch (SQLite::Exception e) {
		this->displayUnrecoverableError();
		return false;
	}
}

void Database::writeSecret(const std::string name, const std::string secret) {
	try {
		SQLite::Statement query(
			/* Database object */ *(db.get()),
			/* SQL Query */
			"INSERT INTO " + this->SECRETS_TABLE + " " +
			"(name, content) VALUES (?, ?)"
		);
		query.bind(1, name);
		query.bind(2, secret);

		query.exec();
	} catch (SQLite::Exception e) {
		Utils::displayError(
			"Could not write secret with name " + name + "!"
		);
	}
}

std::string Database::readSecret(const std::string name) {
	try {
		SQLite::Statement query(
			/* Database object */ *(db.get()),
			/* SQL Query */
			"SELECT content FROM " + this->SECRETS_TABLE + " WHERE name = ?;"
		);
		query.bind(1, name);

		if (!query.executeStep()) {
			return "";
		}

		return query.getColumn(0).getText();
	} catch (SQLite::Exception e) {
		Utils::displayError(
			"Could not read secret " + name + " from database!"
		);
		return "";
	}
}

User& Database::getUser() const {
	return this->user;
}
