/*
 * Copyright (C) 2015 Niko Rosvall <niko@byteptr.com>
 *
 * This file is part of Steel.
 *
 * Steel is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Steel is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Steel.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <sqlite3.h>
#include <sys/stat.h>

#include "database.h"
#include "crypto.h"

static bool file_exists(const char *path)
{
	bool retval = false;
	struct stat buffer;

	if (stat(path, &buffer) == 0)
		retval = true;

	return retval;
}

bool db_init(const char *path, const char *passphrase)
{
	sqlite3 *db;
	char *error = NULL;
	int retval;
	char *sql;

	if(file_exists(path)) {
		fprintf(stderr, "File already exists\n");
		return false;
	}

	retval = sqlite3_open(path, &db);

	if(retval) {
		fprintf(stderr, "Can't initialize: %s\n", sqlite3_errmsg(db));
		return false;
	}

	sql = "create table entries(" \
		"title TEXT," \
		"user  TEXT," \
		"passphrase TEXT," \
		"url TEXT," \
		"notes TEXT," \
		"guid TEXT not null);";

	retval = sqlite3_exec(db, sql, NULL, 0, &error);

	if(retval != SQLITE_OK) {
		fprintf(stderr, "SQL error: %s\n", error);
		sqlite3_free(error);
		sqlite3_close(db);
		return false;
	}

	sqlite3_close(db);

	if(!encrypt_file(path, passphrase)) {
		fprintf(stderr, "Encryption failed\n");
		return false;
	}
	
	return true;
}

bool db_open(const char *path, const char *passphrase)
{

	if(!decrypt_file(path, passphrase)) {
		fprintf(stderr, "Decryption failed\n");
		return false;
	}
	
	
	
	return true;
}

void db_close(const char *passphrase)
{


}

void db_export_text(const char *path)
{

}


