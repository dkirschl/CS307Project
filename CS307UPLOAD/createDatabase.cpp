#include <stdio.h>
#include <sqlite3.h>
#include "tbinclude.h"

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
	int i;
	for(i = 0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
	}
	printf("\n");
	return 0;
}

int main(int argc, char *argv[]){
	createDatabases();
	return 0;
}


int createDatabases()
{
   sqlite3 *db;
   char *zErrMsg = 0;
   int rc;
   char *query;
   char *query2;
   int results;

   rc = sqlite3_open("serverDatabase.db", &db);

   if(rc){
      fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
	  return 0;
   }else{
      fprintf(stderr, "Opened database successfully\n");
   }

   query = "CREATE TABLE IF NOT EXISTS USERS(id INTEGER PRIMARY KEY AUTOINCREMENT, name CHAR(51) NOT NULL, alias CHAR(21) NOT NULL, age INT NOT NULL, gender CHAR(2) NOT NULL, password CHAR(21), picture BLOB, description CHAR(101), firstSport CHAR(31), secondSport CHAR(31), thirdSport CHAR(31));";

   query2 = "CREATE TABLE IF NOT EXISTS EVENTS(id INTEGER PRIMARY KEY AUTOINCREMENT, sport CHAR(31) NOT NULL, location CHAR(31) NOT NULL, date INT NOT NULL, time INT NOT NULL, creatingUser CHAR(21), summary CHAR(101), desiredSkillLevel INT, attendingUsers CHAR(1001), title CHAR(26), numAttending INT, maxNumAttending INT, owningUser INT);";

   results = sqlite3_exec(db,query,callback, 0, &zErrMsg);
   if (results != SQLITE_OK){
	   fprintf(stderr, "SQL error: %s\n", zErrMsg);
	   sqlite3_free(zErrMsg);
   } else {
	   fprintf(stdout, "Table 1 created succesfully\n");
   }

   results = sqlite3_exec(db, query2, callback, 0, &zErrMsg);
   if(results != SQLITE_OK){
	   fprintf(stderr, "SQL error: %s\n", zErrMsg);
   } else {
	   fprintf(stdout, "Table 2 createed succesfully\n");
   }

   sqlite3_close(db);
   return 0;
}
