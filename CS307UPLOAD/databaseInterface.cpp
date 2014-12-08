//DATABASE INTERFACE CODE

#include "tbinclude.h"
using namespace std;

string callback_return;

string callback2;

sqlite3 *db;
bool dbOpen = false;
bool usePipe = true;
bool gameInvite = false;
bool translateIDs = true;

char currentKey[keyL];
char lastKey[keyL];

static int callback(void *data, int argc, char **argv, char **azColName){
	int i;
	fprintf(stdout, "%s: ", (const char*)data);
	fprintf(stdout, "%d\n", argc);
	for(i = 0; i<argc; i++){

		printf("column name = %s\ncallback starts as %s\n", azColName[i], callback_return.c_str());

		if(gameInvite && i == 0){
			callback_return += "2~";
		}

		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");

		if(strcmp(azColName[i],"requestingUser") == 0){
			strcpy(lastKey, argv[i]);
		}

		if(translateIDs && (strcmp(azColName[i],"attendingUsers") == 0 || strcmp(azColName[i],"friendsList") == 0)){
			string aliasList;			
			//argv[i] will be list of ids that need to be translated
			
			aliasList = string(aliasesFromIDs(argv[i]));
			callback_return = callback2.c_str();
			callback_return += aliasList.c_str();
		} else if(strcmp(azColName[i],"targetUser") == 0 || strcmp(azColName[i],"requestingUser") == 0){
			char alias[21];

			getAlias(argv[i], alias);
			alias[20] = '\0';
			printf("You're not crazy. It gets here.\n");
			printf("alias = %s\n", alias);
			printf("key = %s\n", argv[i]);

			callback_return += string(argv[i]);
			callback_return += "~";
			callback_return += string(alias);

			printf("callback_return is now %s\n", callback_return.c_str());

		} else if(strcmp(azColName[i],"inviteGameID") == 0){
			// get game info from id
			getGameDataFromKey(argv[i]);
		} else if(strcmp(azColName[i],"results") == 0 && atoi(argv[i]) == -2){
			removeFriend(currentKey,lastKey);
		} else {
			callback_return += string(argv[i]);
		}
		
		if(i != (argc - 1)){
			callback_return += "~";
		}

		printf("callback changes into %s\n", callback_return.c_str());
	}
	printf("DoneLine\n");
	if(usePipe == true){
		callback_return += "|";
	}
	
	printf("\n");
	return 0;
}
/*
int main(int argc, char *argv[]){
	bool w;
	int a = 1;
	char *b = (char*)&a;
	w = isGoodPass(b, "222");

	return 0;
}
*/

void deleteFriendRequest(int slaveSocket, char * key, char * pass, char * otherKey)
{

		char *zErrMsg = 0;
		int rc;
		int results;
		int file_entry;
		std::string query;
			
		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;
		if(rc){
			fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
		} else {
			fprintf(stderr, "Opened database succesfully\n");
			}
		}
			

		query =  "DELETE FROM FRIENDREQUESTS WHERE requestingUser="+string(key)+" AND targetUser="+string(otherKey)+" AND results=1;";

		const char* data = "Callback function called";

		fprintf(stdout, "%s\n", query.c_str());

		results = sqlite3_exec(db, query.c_str(), callback, (void*) data, &zErrMsg);

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "events checked succesfully\n");
		}

		sqlite3_close(db);
		dbOpen = false;
				
		write(slaveSocket,"ISVALID",7);
		printf("Wrote back ISVALID\n");

}

void dataCheck(int slaveSocket, char * idList)
{
	
		char *zErrMsg = 0;
		int rc;
		int results;
		int file_entry;
		std::string query;
			
		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;
		if(rc){
			fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
		} else {
			fprintf(stderr, "Opened database succesfully\n");
			}
		}
			

		query = "SELECT * FROM EVENTS WHERE id in ("+string(idList)+");";

		const char* data = "Callback function called";

		fprintf(stdout, "%s\n", query.c_str());
		

		resetCallback_return();

		results = sqlite3_exec(db, query.c_str(), callback, (void*) data, &zErrMsg);

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "events checked succesfully\n");
		}

}

void getGameDataFromKey(char * idList)
{
	
		char *zErrMsg = 0;
		int rc;
		int results;
		int file_entry;
		std::string query;
			
		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;
		if(rc){
			fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
		} else {
			fprintf(stderr, "Opened database succesfully\n");
			}
		}
			

		query = "SELECT * FROM EVENTS WHERE id in ("+string(idList)+");";

		const char* data = "Callback function called";

		fprintf(stdout, "%s\n", query.c_str());


		gameInvite = false;
		usePipe = false;
		results = sqlite3_exec(db, query.c_str(), callback, (void*) data, &zErrMsg);
		usePipe = true;
		gameInvite = true;

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "game data from key succesfully\n");
		}

		printf("LOOK HERE! Callback is %s\n", callback_return.c_str());

}


///////////HERE YO

void removeFriend(char * key, char * otherUserKey)
{

		char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;
		int ret;
		int ret2;

		std::string callSave;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		const char* data = "Callback functioin called";
		
		printf("must extract id %s from the current user %s's friends list\n", otherUserKey, key);
	// extract	
		query = "SELECT friendsList USERS WHERE id="+string(otherUserKey)+";";
		
		fprintf(stdout, "%s\n", query.c_str());

		resetCallback_return();

		usePipe = false;
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		usePipe = true;
						
		fprintf(stdout, "RESULTS IS: %d\n", results);
		
		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "extracted friendsList succesfully\n");
		}

////////

		callSave = callback_return.c_str();

		printf("friendsList for user %s = %s\n", key, callSave.c_str());

		ret = stringFindLocation(callSave.c_str(), otherUserKey);

		if(ret < 0){
			// shouldn't unjoin because the alias isn't in
			printf("DELETE FRIEND ERROR: Key %s was not friends with user %s\n", otherUserKey, key);			
			return;
		}

		std::string newFriendsList;
		newFriendsList = callSave.substr(0,ret) + callSave.substr(ret+keyL);
	
		printf("New friendsList after extracting key %s at index %d = %s\n", otherUserKey, ret, newFriendsList.c_str()); 

		query = "UPDATE USERS SET friendsList ='"+string(newFriendsList.c_str())+"' WHERE id="+string(key)+";";
	
		results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "updated friendsList succesfully, motherfucker!\n");
		}	
	
		query = "DELETE FROM FRIENDREQUESTS WHERE requestingUser="+string(otherUserKey)+" AND targetUser="+string(key)+" AND results=-2;";
		
		fprintf(stdout, "%s\n", query.c_str());

		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);

		fprintf(stdout, "RESULTS IS: %d\n", results);
		
		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "deleted delete request succesfully\n");
		}


}


void dataGetFR(int slaveSocket, char * key, char * password)
{
	string query;	
	char *zErrMsg = 0;
	int rc;
	int results;
	int file_entry;

	if(!dbOpen){
		rc = sqlite3_open("serverDatabase.db", &db);
		dbOpen = true;
		if(rc){
			fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
		} else {
			fprintf(stderr, "Opened database succesfully\n");
		}
	}

	const char* data = "Callback function called";
	
	strcpy(currentKey,key);

	query = "SELECT requestingUser, results FROM FRIENDREQUESTS WHERE targetUser="+string(key)+" AND results=-2;";
	
	results = sqlite3_exec(db, query.c_str(), callback, (void*) data, &zErrMsg);
	
	if(results != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	} else {
		fprintf(stdout, "Delete friends handled succesfully hopefully\n");
	}
		
	resetCallback_return();
	callback_return = "|";
	
	query = "SELECT results, targetUser FROM FRIENDREQUESTS WHERE requestingUser="+string(key)+" AND results=1;";

	results = sqlite3_exec(db, query.c_str(), callback, (void*) data, &zErrMsg);

	if(results != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	} else {
		fprintf(stdout, "accepted friend requests searched succesfully\n");
	}
		
	query = "SELECT results, requestingUser FROM FRIENDREQUESTS WHERE targetUser="+string(key)+" AND results=0;";

	results = sqlite3_exec(db, query.c_str(), callback, (void*) data, &zErrMsg);

	if(results != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	} else {
		fprintf(stdout, "Friend requests searched succesfully\n");
	}

	if(callback_return.length() <= 1){
		resetCallback_return();
	}

	fprintf(stdout, "callback_return: %s\n", callback_return.c_str());
			
	sqlite3_close(db);
	dbOpen = false;

	write(slaveSocket, callback_return.c_str(), callback_return.length());
	fprintf(stdout, "Wrote back\n");
		
}
	
void dataGetG(int slaveSocket, char * key, char * password)
{
	string query;	
	char *zErrMsg = 0;
	int rc;
	int results;
	int file_entry;

	if(!dbOpen){
		rc = sqlite3_open("serverDatabase.db", &db);
		dbOpen = true;
		if(rc){
			fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
		} else {
			fprintf(stderr, "Opened database succesfully\n");
		}
	}

	const char* data = "Callback function called";
			
	resetCallback_return();
	callback_return = "|";
	
	query = "SELECT requestingUser, inviteGameID FROM GAMEINVITES WHERE targetUser="+string(key)+";";

	printf("query = %s", query.c_str());

	gameInvite = true;
	results = sqlite3_exec(db, query.c_str(), callback, (void*) data, &zErrMsg);
	gameInvite = false;

	if(results != SQLITE_OK){
		fprintf(stderr, "SQL error: %s\n", zErrMsg);
	} else {
		fprintf(stdout, "Friend requests searched succesfully\n");
	}

	fprintf(stdout, "callback_return: %s\n", callback_return.c_str());
			
	sqlite3_close(db);
	dbOpen = false;

	write(slaveSocket, callback_return.c_str(), callback_return.length());
	fprintf(stdout, "Wrote back\n");
		
	

}

void dataInviteGame(int slaveSocket, char * key, char * pass, char *guest, char * event)
{
	if(isValidValue(pass) && isGoodPass(key, pass) && isValidValue(guest) && isValidValue(event))
	{
	
		char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;
		int ret;

		std::string callSave;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		const char* data = "Callback functioin called";

		query = "INSERT INTO GAMEINVITES(requestingUser, targetUser, inviteGameID) VALUES("+string(key)+","+string(guest)+","+string(event)+");";
		
		fprintf(stdout, "%s\n", query.c_str());
		
		resetCallback_return();
		
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
			
		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "invite user to game succesfully\n");
		}

		sqlite3_close(db);
		dbOpen = false;
				
		write(slaveSocket,"ISVALID",7);
		printf("Wrote back ISVALID\n");

///////////////////////////////////////////////////////////////////////////////////////////////
	}
	else
	{
		write(slaveSocket,"INVALID",7);
		printf("Wrote back INVALID\n");
	}
}

void deleteGameInvite(int slaveSocket, char * key, char * pass, char * event){
	if(isValidValue(pass) && isGoodPass(key, pass) && isValidValue(event))
	{
	
		char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;
		int ret;

		std::string callSave;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		const char* data = "Callback functioin called";

		query = "DELLETE FROM GAMEINVITES WHERE targetUser="+string(key)+" AND inviteGameID="+string(event)+";";
		
		fprintf(stdout, "%s\n", query.c_str());
		
		resetCallback_return();
		
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
			
		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "deleted game invite succesfully\n");
		}

		sqlite3_close(db);
		dbOpen = false;
				
		write(slaveSocket,"ISVALID",7);
		printf("Wrote back ISVALID\n");

///////////////////////////////////////////////////////////////////////////////////////////////
	}
	else
	{
		write(slaveSocket,"INVALID",7);
		printf("Wrote back INVALID\n");
	}

}

////////////////WE MAY HAVE STOPPED HERE!!!


//INPUT: 	slaveSocket, password, name, alias, age, gender
//IDEA:		Accept input and create a user based on provided info.
//OUTPUT:	Write confirmation back to user if successful.
//RETURN:	NONE
void dataCreateUser(int slaveSocket,char * password, char * name, char * alias, char * age, char * gender)
{
	if(isValidValue(password) && isValidValue(name) && isValidValue(alias) && isValidValue(age) && isValidValue(gender) && !checkAlias(alias))
	{
		
		//add user to the database and return the key from the database if there is an error then return -1
		write(slaveSocket,"ISVALID",7);
		printf("Wrote back ISVALID\n");


		//sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		std::string query;
		std::string query2;
		int results;
		int file_entry;
		char* user;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		// check if alias is unique
		//

		const char* data = "Callback functioin called";

		query = "INSERT INTO USERS (password, name, alias, age, gender, firstSport, secondSport, thirdSport,friendsList, preferences) VALUES('"+string(password)+"','"+ string(name) + "','" + string(alias) + "','" + string(age) + "','" + string(gender) + "','  ','  ','  ','',1);";
		query2 = "SELECT seq FROM sqlite_sequence WHERE name='USERS';";
		//query2 = "SELECT last_row_id();";
fprintf(stdout, "%s\n", query.c_str());
		
		results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);
fprintf(stdout, "RESULTS IS: %d\n", results);

		resetCallback_return();
		usePipe = false;
		file_entry = sqlite3_exec(db, query2.c_str(), callback, (void*) data, &zErrMsg);
		usePipe = true;

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "User inserted succesfully\n");
		}
		if(file_entry != SQLITE_OK){
			fprintf(stderr, "SQL 2 error: %s\n", zErrMsg);
		} else {
		//	user = user_id;
			fprintf(stdout, "User ID found succesfully\n");
		}
		fprintf(stdout, "User Id: %s\n", callback_return.c_str());

		sqlite3_close(db);
		dbOpen = false;


		
		write(slaveSocket, callback_return.c_str(), callback_return.length());
		fprintf(stdout, "Wrote back\n");
	


	}
	else
	{	
		write(slaveSocket,"INVALID",7);
		printf("Wrote back INVALID\n");
	}
}



void dataDeleteUser(int slaveSocket, char * key, char * password)
{
	if(isValidValue(password) && isGoodPass(key, password))
	{
		write(slaveSocket,"ISVALID",7);
		printf("Wrote back ISVALID\n");
		//database stuff

			//sqlite3 *db;
			char *zErrMsg = 0;
			int rc;
			std::string query;
			int results;

			if(!dbOpen){
				rc = sqlite3_open("serverDatabase.db", &db);
				dbOpen = true;
				
				if(rc){
					fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
				} else {
					fprintf(stderr, "Opened database succesfully\n");
				}
			}

			const char* data = "Callback functioin called";

			query = "DELETE from USERS where id="+string(key)+";"; 
			
			fprintf(stdout, "%s\n", query.c_str());
			results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);
			fprintf(stdout, "RESULTS IS: %d\n", results);
			
			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "User deleted succesfully\n");
			}
			
			sqlite3_close(db);
			dbOpen = false;
			
			fprintf(stdout, "Finished action\n");

	}
	else
	{
		write(slaveSocket,"INVALID",7);
		printf("Wrote back INVALID\n");
	}
}

void dataJoinEvent(int slaveSocket, char * key, char * password, char * evKey, int printBack)
{
	if(isValidValue(password) && isGoodPass(key, password) && isValidValue(evKey))
	{
	
		char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;
		//char alias[21];
		int ret;
		std::string callSave;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		const char* data = "Callback functioin called";
		
		printf("eventkey: %s\n",evKey);
		lockRow(evKey,key);

		printf("Row locked\n");

		//getAlias(key, alias);

		//printf("Alias from key %s = %s\n", key,alias);
		

		query = "SELECT attendingUsers FROM EVENTS where id="+string(evKey)+";";

		
		fprintf(stdout, "%s\n", query.c_str());
		
		resetCallback_return();
		
		translateIDs = false;
		usePipe = false;
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		usePipe = true;
		translateIDs = true;
		
				
		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "extracted attendingUsers succesfully\n");
		}

		// callback return is attendingUsers

		callSave = callback_return.c_str();

		printf("AttendingUsers returned from event %s = %s\n", evKey, callSave.c_str());

		ret = stringFindLocation(callSave.c_str(), key);

		fprintf(stdout, "User Id: %s\n", callback_return.c_str());

		printf("ret = %d\n", ret);

		if(ret >= 0){
			// shouldn't join because the alias is already in
			printf("User attempted to join an event they are already in\n");
			write(slaveSocket, "INVALID",7);
			unlockRow(evKey);
			sqlite3_close(db);
			dbOpen = false;
			return;
		}

		printf("User is not in event to join\n");

		callSave += string(key) + "*";

		printf("New attendingUser string after appending = %s\n", callSave.c_str());
		
		query = "UPDATE EVENTS SET numAttending = numAttending + 1 WHERE id="+string(evKey)+" AND numAttending < maxNumAttending;";
		
		results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "incremented numAttending succesfully\n");
		}
		
		// incremented

		query = "UPDATE EVENTS SET attendingUsers='"+string(callSave.c_str())+"' WHERE id="+string(evKey)+";";
		
		fprintf(stdout, "%s\n", query.c_str());
		
		resetCallback_return();
		
		results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);
		
		fprintf(stdout, "RESULTS IS: %d\n", results);
		
		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "attendingUsers updated succesfully\n");
		}
		
		printf("Attempting to unlock event %s\n", evKey);
		unlockRow(evKey);
		printf("Unlocked event %s\n", evKey);
		
		sqlite3_close(db);
		dbOpen = false;
				
		write(slaveSocket,"ISVALID",7);
		printf("Wrote back ISVALID\n");

		if(printBack == 1)
		{
			write(slaveSocket,evKey,strlen(evKey));
		}
		//database stuff
	}
	else
	{
		write(slaveSocket,"INVALID",7);
		printf("Wrote back INVALID\n");
	}
}

void dataUnJoinEvent(int slaveSocket, char * key, char * password, char * evKey)
{
	if(isValidValue(password) && isGoodPass(key, password) && isValidValue(evKey))
	{
		
char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;
		//char alias[21];
		int ret;
		std::string callSave;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		const char* data = "Callback functioin called";
		
		printf("Attempting to lock event %s\n", evKey);

		lockRow(evKey,key);

		printf("locked event %s\n", evKey);


		//getAlias(key, alias);
		//printf("Alias from key %s = %s\n", key, alias);

		query = "SELECT attendingUsers FROM EVENTS where id="+string(evKey)+";";
		
		fprintf(stdout, "%s\n", query.c_str());

		resetCallback_return();
		
		usePipe = false;
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		usePipe = true;
		
		fprintf(stdout, "RESULTS IS: %d\n", results);
		
		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "extracted attendingUsers succesfully\n");
		}

		// callback return is attendingUsers

		callSave = callback_return.c_str();

		printf("AttendingUsers for event %s = %s\n", evKey, callSave.c_str());

		ret = stringFindLocation(callSave.c_str(), key);

		if(ret < 0){
			// shouldn't unjoin because the alias isn't in
			printf("UNJOIN ERROR: Key %s was not attending the event %s\n", key, evKey);			
			write(slaveSocket, "INVALID",7);
			printf("Wrote back INVALID\n");
			unlockRow(evKey);
			sqlite3_close(db);
			dbOpen = false;
			return;
		}

		std::string newAttend;
		newAttend = callSave.substr(0,ret) + callSave.substr(ret+keyL);
		
		printf("New attendingUsers after extracting key %s at index %d = %s\n", key, ret, newAttend.c_str()); 

		query = "UPDATE EVENTS SET numAttending = numAttending - 1 WHERE id="+string(evKey)+" AND numAttending < maxNumAttending;";
		
		results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "decremented numAttending succesfully\n");
		}
		
		// incremented

		query = "UPDATE EVENTS SET attendingUsers='"+string(newAttend.c_str())+"' WHERE id="+string(evKey)+";";
		
		fprintf(stdout, "%s\n", query.c_str());
		resetCallback_return();
		
		results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);
		
		fprintf(stdout, "RESULTS IS: %d\n", results);
		
		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "attendingUsers updated succesfully\n");
		}
		
		printf("Attempting to unlock event %s\n", evKey);
		unlockRow(evKey);
		printf("Unlocked event %s\n", evKey);
		
		sqlite3_close(db);
		dbOpen = false;	
		
		write(slaveSocket,"ISVALID",7);
		printf("Wrote back ISVALID\n");
		//database stuff
	}
	else
	{
		write(slaveSocket,"INVALID",7);
		printf("Wrote back INVALID\n");
	}
}


//INPUT:	slaveSocket, key, password, sport, location, date, time, summary, skill level
//IDEA:		Accept into and create an event based on provided info.
//OUTPUT:	Write confirmation back to user if successful.
//RETURN:	NONE
void dataCreateEvent(int slaveSocket,char * key,char * password,char * sport,char * location,char * date,char * time,char * summary,char * compete, char * title, char * attendM)
{
	if(isValidValue(password) && isValidValue(sport) && isValidValue(location) && isValidValue(date) && isValidValue(time) && isValidValue(summary) && isValidValue(compete) && isValidValue(title) && isValidValue(attendM))
	{
		if(isGoodPass(key, password) == false)
		{
			write(slaveSocket,"INVALID",7);
			printf("Wrote back INVALID\n");
			printf("Good pass failed\n");
		}
		else
		{
		
		//Do stuff ************************

			//write(slaveSocket,"ISVALID",7);
			//printf("Wrote back ISVALID\n");

			//sqlite3 *db;
			char *zErrMsg = 0;
			char *zErrMsg2 = 0;
			int rc;
			std::string query;
			std::string query2;
			int results;
			int file_entry;
			char* event;

			if(!dbOpen){
				rc = sqlite3_open("serverDatabase.db", &db);
				dbOpen = true;

				if(rc){
					fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
				} else {
					fprintf(stderr, "Opened database succesfully\n");
				}
			}

			const char* data = "Callback functioin called";

			query = "INSERT INTO EVENTS (sport, location, date, time, summary,desiredSkillLevel, creatingUser, title, numAttending, maxNumAttending, owningUser,attendingUsers) VALUES('"+string(sport)+"','"+ string(location) + "','" + string(date) + "','" + string(time) + "','" + string(summary) + "','" + string(compete) + "'," + string(key)+",'"+string(title)+"','"+string("0")+"','"+string(attendM)+"',0,'');";

			query2 = "SELECT seq FROM sqlite_sequence WHERE name='EVENTS';";
			//query2 = "SELECT last_row_id();";\

	fprintf(stdout, "%s\n", query.c_str());
			usePipe = false;
			results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);
			usePipe = true;
	fprintf(stdout, "RESULTS IS: %d\n", results);
			resetCallback_return();
			usePipe = false;
			file_entry = sqlite3_exec(db, query2.c_str(), callback, (void*) data, &zErrMsg2);
			usePipe = true;
			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "Event inserted succesfully\n");
			}
			if(file_entry != SQLITE_OK){
				fprintf(stderr, "SQL 2 error: %s\n", zErrMsg2);
			} else {
			//	user = user_id;
				fprintf(stdout, "Event ID found succesfully\n");
			}
			fprintf(stdout, "Event Id: %s\n", callback_return.c_str());

			sqlite3_close(db);
			dbOpen = false;

			//write(slaveSocket, callback_return.c_str(), callback_return.length());
			fprintf(stdout, "Wrote back\n");

			fprintf(stdout, "key=%s\n", key);
			fprintf(stdout, "evKey=%s\n", callback_return.c_str());
			
			char evKeyinput[5];
			strcpy(evKeyinput, callback_return.c_str());
			printf("eventKey: %s", evKeyinput);			

			dataJoinEvent( slaveSocket, key, password, evKeyinput, 1);
			
		}
	}
	else
	{
		write(slaveSocket,"INVALID",7);
		printf("Wrote back INVALID\n");
	}
}

//INPUT:	slaveSocket, key, password, name, alias, age, gender, desc.
//IDEA:		Accept info, check if it is valid. If valid change relevent user data.
//OUTPUT:	Confirmation back if successful.
//RETURN:	NONE
void dataUpdateUser(int slaveSocket,char * key,char * password,char * name,char * alias,char * age,char * gender,char * desc, char * newPass, char * newPref)
{
	if(isValidValue(password) && isValidValue(name) && isValidValue(alias) && isValidValue(age) && isValidValue(gender) && isSwearFree(desc) && isValidValue(newPass))
	{
		if(isGoodPass(key, password) == false)
		{
			write(slaveSocket,"INVALID",7);
			printf("Wrote back INVALID\n");
			printf("Good pass failed\n");
		}
		else
		{
			//sqlite3 *db;
			char *zErrMsg = 0;
			int rc;
			std::string query;
			int results;
			int aliasFlag = 1;


			if(!dbOpen){
				rc = sqlite3_open("serverDatabase.db", &db);
				dbOpen = true;

				if(rc){
					fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
				} else {
					fprintf(stderr, "Opened database succesfully\n");
				}			
			}

			const char* data = "Callback functioin called";
////////////////////////////////////

			query = "SELECT alias FROM USERS WHERE id="+string(key)+";";


			resetCallback_return();

			usePipe = false;
			results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
			usePipe = true;

			printf("callback return = %s\nalias = %s\n", callback_return.c_str(), alias);
		
			if(strncmp(callback_return.c_str(), alias, 21) == 0){
				aliasFlag = 0;
				printf("alias is the same as before\n");
			}

			if(aliasFlag == 1){
				printf("alias has changed\n");
				if(checkAlias(alias)){
					write(slaveSocket, "INVALID ALIAS", 13);
					sqlite3_close(db);
					return;
				}
			}


////////////////////////////////////
			query = "UPDATE USERS SET name='"+string(name)+"' alias='"+string(alias)+"' age='"+string(age)+"' gender='"+string(gender)+"' description='"+string(desc)+"' password='"+ string(newPass) +"' preferences="+string(newPref)+" WHERE id="+string(key)+";"; 
			
			fprintf(stdout, "%s\n", query.c_str());
			results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);
			fprintf(stdout, "RESULTS IS: %d\n", results);
			
			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "Event deleted succesfully\n");
			}
			
			sqlite3_close(db);
			dbOpen = false;

			fprintf(stdout, "Finished action\n");

			write(slaveSocket,"ISVALID",7);
			printf("Wrote back ISVALID\n");
		}
	}
	else
	{
		write(slaveSocket,"INVALID",7);
		printf("Wrote back INVALID\n");
	}
}

//INPUT:	slaveSocket, key, password, event key, sport, location, date, time, summary, skill level
//IDEA:		Accept info, check if it is valid. If valid, change relevent event data.
//OUTPUT:	Confirmation back if successful.
//RETURN:	NONE
void dataUpdateEvent(int slaveSocket,char * key,char * password,char * evKey,char * sport,char * location,char * date,char * time,char * summary,char * skill, char * title, char * attendM)
{
	if(isValidValue(password) && isValidValue(sport) && isValidValue(location) && isValidValue(date) && isValidValue(time) && isSwearFree(summary) && isValidValue(skill) && isValidValue(title) && isValidValue(attendM))
	{
		if(isGoodPass(key, password) == false || isCorrectUser(key,evKey) == false)
		{
			write(slaveSocket,"INVALID",7);
			printf("Wrote back INVALID\n");
			printf("Good pass failed\n");
		}
		else
		{
		
			//sqlite3 *db;
			char *zErrMsg = 0;
			int rc;
			std::string query;
			int results;

			if(!dbOpen){
				rc = sqlite3_open("serverDatabase.db", &db);
				dbOpen = true;

				if(rc){
					fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
				} else {
					fprintf(stderr, "Opened database succesfully\n");
				}			
			}

			const char* data = "Callback functioin called";

			query = "UPDATE EVENTS SET sport='"+string(sport)+"' location='"+string(location)+"' date="+string(date)+" time="+string(time)+" summary='"+string(summary)+"' skill='"+ string(skill) +"' title='"+string(title)+ +"' maxNumAttending="+string(attendM) +" WHERE id="+string(evKey)+";"; 
			
			fprintf(stdout, "%s\n", query.c_str());
			results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);
			fprintf(stdout, "RESULTS IS: %d\n", results);
			
			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "Event updated succesfully\n");
			}
			
			sqlite3_close(db);
			dbOpen = false;

			fprintf(stdout, "Finished action\n");




			write(slaveSocket,"ISVALID",7);
			printf("Wrote back ISVALID\n");
		}
	}
	else
	{
		write(slaveSocket,"INVALID",7);
		printf("Wrote back INVALID\n");
	}
}

//INPUT:	Key, password, Event key
//IDEA:		Accept info, check if valid. If valid, delete specified event.
//OUTPUT:	Confirmation of success or failure.
//RETURN:	NONE
void dataDeleteEvent(int slaveSocket,char * key,char * password,char * evKey)
{
	if(isGoodPass(key, password) == false || isCorrectUser(key,evKey) == false)
	{
		write(slaveSocket,"INVALID",7);
		printf("Wrote back INVALID\n");
		printf("Good pass failed\n");
	}
	else
	{

		write(slaveSocket,"ISVALID",7);
		printf("Wrote back ISVALID\n");

/////////////////////////////////////////////////////////////////
			
			//sqlite3 *db;
			char *zErrMsg = 0;
			int rc;
			std::string query;
			int results;
			int file_entry;

			if(!dbOpen){
				rc = sqlite3_open("serverDatabase.db", &db);
				dbOpen = true;

				if(rc){
					fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
				} else {
					fprintf(stderr, "Opened database succesfully\n");
				}
			}
			const char* data = "Callback functioin called";

			query = "DELETE from EVENTS where id="+string(evKey)+";"; 
			
			fprintf(stdout, "%s\n", query.c_str());
			results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);
			fprintf(stdout, "RESULTS IS: %d\n", results);
			
			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "Event deleted succesfully\n");
			}
			
			sqlite3_close(db);
			dbOpen = false;

			fprintf(stdout, "Finished action\n");

////////////////////////////////////////////////////////////////		
		
	}
}

//INPUT:	Sport, location, date, time, skill level
//IDEA:		Accept info, check for valid inputs. For valid inputs search the database.  
//OUTPUT:	Events matching request.
//RETURN:	NONE
void dataGetEvent(int slaveSocket,char * sport,char * location,char * date,char * time,char * skill, char * title, char * otherD, char * otherT)
{

	write(slaveSocket,"ISVALID",7);
	printf("Wrote back ISVALID\n");


	string query = "SELECT * FROM EVENTS";	//need to return more
	int flag = 0;
	if(isValidValue(sport))
	{
		if(flag == 0){
			query += " WHERE ";
		}

		query += " sport ='" + string(sport)+ "' ";
		flag = 1;
	}
	if(isValidValue(location))
	{
		if(flag == 0){
			query += " WHERE ";
		}

		if(flag == 1)
		{
			query += " AND ";
		}	
		query += " location ='" + string(location) + "' ";
		flag = 1;
	}

	if(isValidValue(date))
	{
		if(flag == 0){
			query += " WHERE ";
		}

		if(flag == 1)
		{
			query += " AND ";
		}	
		query += " date =>" + string(date) + " ";
		flag = 1;
	}

	if(isValidValue(time))
	{
		if(flag == 0){
			query += " WHERE ";
		}

		if(flag == 1)
		{
			query += " AND ";
		}	
		query += " time =>" + string(time) + " ";
		flag = 1;
	}

	if(isValidValue(otherD))
	{
		if(flag == 0){
			query += " WHERE ";
		}

		if(flag == 1)
		{
			query += " AND ";
		}	
		query += " date <=" + string(otherD) + " ";
		flag = 1;
	}

	if(isValidValue(otherT))
	{
		if(flag == 0){
			query += " WHERE ";
		}

		if(flag == 1)
		{
			query += " AND ";
		}	
		query += " time <=" + string(otherT) + " ";
		flag = 1;
	}

	if(isValidValue(title))
	{
		if(flag == 0){
			query += " WHERE ";
		}

		if(flag == 1)
		{
			query += " AND ";
		}	
		query += " title ='" + string(title) + "' ";
		flag = 1;
	}

	if(isValidValue(skill))
	{
		if(flag == 0){
			query += " WHERE ";
		}

		if(flag == 1)
		{
			query += " AND ";
		}	
		query += " skill ='" + string(skill) + "' ";
		flag = 1;
	}

	query += ";";

///////////////////////////////////////////////////////////////////////

			//sqlite3 *db;
			char *zErrMsg = 0;
			int rc;
			int results;
			int file_entry;

			if(!dbOpen){
				rc = sqlite3_open("serverDatabase.db", &db);
				dbOpen = true;
				if(rc){
					fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
				} else {
					fprintf(stderr, "Opened database succesfully\n");
				}
			}

			const char* data = "Callback function called";

			fprintf(stdout, "%s\n", query.c_str());
			
			resetCallback_return();
			callback_return = "|";
			
			translateIDs = false;
			results = sqlite3_exec(db, query.c_str(), callback, (void*) data, &zErrMsg);
			translateIDs = true;

			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "Event inserted succesfully\n");
			}
		
			fprintf(stdout, "callback_return: %s\n", callback_return.c_str());
			
			sqlite3_close(db);
			dbOpen = false;

			write(slaveSocket, callback_return.c_str(), callback_return.length());
			fprintf(stdout, "Wrote back\n");
	
///////////////////////////////////////////////////////////////////////////////////////	


}

void dataLogOn(int slaveSocket,char * alias,char * password)
{
	if(isValidValue(password) && isValidValue(alias))
	{
		if(isGoodSet(alias, password) == false) //do things
		{
			write(slaveSocket,"INVALID",7);
		}
		else
		{
		
		//Do stuff ************************

			write(slaveSocket,"ISVALID",7);
			printf("Wrote back ISVALID\n");

		//sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		// check if alias is unique
		//

		const char* data = "Callback functioin called";

		query = "SELECT id, age, gender, name, description FROM USERS WHERE alias='" + string(alias)+ "');";

		fprintf(stdout, "%s\n", query.c_str());

		resetCallback_return();

		usePipe = false;
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		usePipe = true;

		fprintf(stdout, "RESULTS IS: %d\n", results);

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "User inserted succesfully\n");
		}
		

		sqlite3_close(db);
		dbOpen = false;

		write(slaveSocket, callback_return.c_str(), callback_return.length());
		fprintf(stdout, "Wrote back\n");

		}
	}
	else
	{
		write(slaveSocket,"INVALID",7);


	}
}


void updateTopSports(int slaveSocket, char * key, char * password, char * firstSport, char * secondSport, char * thirdSport)
{
	if(isValidValue(password))
	{
		if(isGoodPass(key, password) == false)
		{
			write(slaveSocket,"INVALID",7);
		}
		else
		{
			//sqlite3 *db;
			char *zErrMsg = 0;
			int rc;
			std::string query;
			int results;

			if(!dbOpen){
				rc = sqlite3_open("serverDatabase.db", &db);
				dbOpen = true;

				if(rc){
					fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
				} else {
					fprintf(stderr, "Opened database succesfully\n");
				}			
			}

			const char* data = "Callback function called";

			query = "UPDATE USERS SET firstSport='"+string(firstSport)+"' secondSport='"+string(secondSport)+"' thirdSport='"+string(thirdSport)+"' WHERE id="+string(key)+";"; 
			
			fprintf(stdout, "%s\n", query.c_str());
			
			usePipe = false;
			results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);
			usePipe = true;
			
			fprintf(stdout, "RESULTS IS: %d\n", results);
			
			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "Updated top sports succesfully\n");
			}
			
			sqlite3_close(db);
			dbOpen = false;

			fprintf(stdout, "Finished action\n");

			write(slaveSocket,"ISVALID",7);
			printf("Wrote back ISVALID\n");
		}
	}
	else
	{
		write(slaveSocket,"INVALID",7);
	}
}



void getCreatedNumAttending(int slaveSocket, char * userKey){

	if(isValidValue(userKey))
	{
		
		//Do stuff ************************

			write(slaveSocket,"ISVALID|",8);
			printf("Wrote back ISVALID\n");

		//sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		// check if alias is unique
		//

		const char* data = "Callback functioin called";

		query = "SELECT id, numAttending FROM EVENTS WHERE creatingUser=" + string(userKey)+ ";";

		fprintf(stdout, "%s\n", query.c_str());

		resetCallback_return();

		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);

		fprintf(stdout, "RESULTS IS: %d\n", results);

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "User checked created succesfully\n");
		}
		

		sqlite3_close(db);
		dbOpen = false;

		write(slaveSocket, callback_return.c_str(), callback_return.length());
		fprintf(stdout, "Wrote back\n");

	}
	else
	{
		write(slaveSocket,"INVALID",7);


	}	
	
	
	
	
	
}

void dataAddFriend(int slaveSocket, char * key, char * password, char * otherUser){
	
	if(isValidValue(password) && isGoodPass(key, password) && isValidValue(otherUser))
	{
	
		char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;
		char otherUserKey[keyL];
		int ret;
		int ret2;

		std::string callSave;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		const char* data = "Callback functioin called";
		
		printf("otherUser alias: %s\n",otherUser);
		//lockRow(evKey,key);

		//printf("Row locked\n");

		getKeyFromAlias(otherUser, otherUserKey);

		printf("Key from alias %s = %s\n", otherUserKey,otherUser);

		query = "SELECT results FROM FRIENDREQUESTS where requestingUser="+string(key)+" AND targetUser="+string(otherUserKey)+";";
		
		fprintf(stdout, "%s\n", query.c_str());
		
		resetCallback_return();
		
		usePipe = false;
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		usePipe = true;
		
				
		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "tested if request already exists succesfully\n");
		}

		
		if(callback_return.length() > 0){
			// request already exists
			printf("User %s requesting User %s already exists in the table\n return from the select: %s\n", key, otherUserKey, callback_return.c_str());
			write(slaveSocket, "INVALID", 7);
			sqlite3_close(db);
			dbOpen = false;
			return;
		}
		
		query = "SELECT results FROM FRIENDREQUESTS where requestingUser="+string(otherUserKey)+" AND targetUser="+string(key)+";";

		fprintf(stdout, "%s\n", query.c_str());

		resetCallback_return();
	
		usePipe = false;
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		usePipe = true;

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "tested if opposite request exists succesfully\n");
		}

		if(callback_return.length() > 0){
			// opposite request exists

			printf("opposite request exists\n callback_return = %s\n", callback_return.c_str());

			query = "UPDATE FRIENDREQUESTS SET results=1 WHERE requestingUser="+string(otherUserKey)+" AND targetUser="+string(key)+";";
			
			fprintf(stdout, "%s\n", query.c_str());

			results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);

			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "accepted friend request to user %s from user %s succesfully\n", key, otherUserKey);
			}

			query = "SELECT friendsList FROM USERS where id="+string(key)+";";

			fprintf(stdout, "%s\n", query.c_str());
			
			usePipe = false;
			results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
			usePipe = true;

			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "extracted friendsList\n");
				printf("friendsList = %s\n", callback_return.c_str());
			}			

			ret = stringFindLocation(callback_return.c_str(), otherUserKey);

			if(ret >= 0){
				// user is already in friendsList, which probably means a double accept
				printf("ret = %d\n", ret);

				printf("User attempted to accept a friend request from a current friend\n");
				write(slaveSocket, "INVALID", 7);
				sqlite3_close(db);
				dbOpen = false;
				return; 
			}

			callSave = callback_return.c_str();

			callSave += string(key) + "*";

			printf("New friendsList string after appending = %s\n", callSave.c_str());

			query = "UPDATE USERS SET friendsList = '"+ string(callSave.c_str()) +"' where id="+string(key)+";";

			fprintf(stdout, "%s\n", query.c_str());
		
			resetCallback_return();
		
			results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);
		
			fprintf(stdout, "RESULTS IS: %d\n", results);
		
			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "attendingUsers updated succesfully\n");
			}
			
		} else {
			// opposite request didn't exist

			query = "SELECT friendsList FROM USERS WHERE id = "+string(key)+";";

			results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);

			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "extracted friendsList\n");
				printf("requesting User's friendsList = %s\n", callback_return.c_str());
			}			

			ret = stringFindLocation(callback_return.c_str(), otherUserKey);

			printf("requesting user ret = %d\n", ret);

			query = "SELECT friendsList FROM USERS WHERE id = "+string(otherUserKey)+";";

			results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);

			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "extracted friendsList\n");
				printf("target user's friendsList %s\n", callback_return.c_str());
			}			

			ret2 = stringFindLocation(callback_return.c_str(), key);

			printf("target user's ret = %d\n", ret2);

			if(ret >= 0 || ret2 >= 0){
				// already friends
				printf("User attempted to request a current friend\n");
				write(slaveSocket, "INVALID", 7);
				sqlite3_close(db);
				dbOpen = false;
				return; 				
			}		

			query = "INSERT INTO FRIENDREQUESTS (requestingUser, targetUser, results) VALUES("+string(key)+","+string(otherUserKey)+",0);";

			results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);

			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "added request to FRIENDREQUESTS\n");
			}				
		}


		sqlite3_close(db);
		dbOpen = false;
				
		write(slaveSocket,"ISVALID",7);
		printf("Wrote back ISVALID\n");

///////////////////////////////////////////////////////////////////////////////////////////////
	}
	else
	{
		write(slaveSocket,"INVALID",7);
		printf("Wrote back INVALID\n");
	}
	
	
}


void dataDeleteFriend(int slaveSocket, char * key, char * password, char * otherUser){
	if(isValidValue(password) && isGoodPass(key, password) && isValidValue(otherUser))
	{
		
		char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;
		//char alias[21];
		char otherUserKey[keyL];
		int ret;
		std::string callSave;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		const char* data = "Callback functioin called";


		getKeyFromAlias(otherUser, otherUserKey);
		printf("key from alias %s = %s\n", otherUserKey, otherUser);

		query = "SELECT results FROM FRIENDREQUESTS where requestingUser="+string(key)+" AND targetUser="+string(otherUserKey)+";";
		
		fprintf(stdout, "%s\n", query.c_str());

		resetCallback_return();
		
		usePipe = false;
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		usePipe = true;
		
		fprintf(stdout, "RESULTS IS: %d\n", results);
		
		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "check for exact request succesfully\n");
		}

		// callback return is attendingUsers

		if(callback_return.length() > 0){

			printf("exact request exists from user %s to user %s\n", key, otherUserKey);			


			printf("atoi return from %s = %d\n", callback_return.c_str(), atoi(callback_return.c_str()));
			
			if(atoi(callback_return.c_str()) == 1){
				printf("and that does = 1, so it has been accepted. Add new delete request to FRIENDREQUEST\n");	
				
				
			// add to FRIENDREQUESTS	
				query = "INSERT INTO FROM FRIENDREQUESTS (requestingUser, targetUser, results) VALUES("+string(key)+","+string(otherUserKey)+",-2);";
		
				fprintf(stdout, "%s\n", query.c_str());

				resetCallback_return();
		
				results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		
				fprintf(stdout, "RESULTS IS: %d\n", results);
		
				if(results != SQLITE_OK){
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
				} else {
					fprintf(stdout, "inserted delete friend request succesfully\n");
				}
				
			}
			
			// delete the exact request

			query = "DELETE FROM FRIENDREQUESTS WHERE requestingUser="+string(key)+" AND targetUser="+string(otherUserKey)+" AND results != -2;";

			fprintf(stdout, "%s\n", query.c_str());

			resetCallback_return();
		
			results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		
			fprintf(stdout, "RESULTS IS: %d\n", results);
		
			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "exact request deleted succesfully\n");
			}

			sqlite3_close(db);
			dbOpen = false;
				
			write(slaveSocket,"ISVALID",7);
			printf("Wrote back ISVALID\n");
			
			return;
		}

		// exact request did not exist

		query = "SELECT results FROM FRIENDREQUESTS where requestingUser="+string(otherUserKey)+" AND targetUser="+string(key)+";";
		
		fprintf(stdout, "%s\n", query.c_str());

		resetCallback_return();
		
		usePipe = false;
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		usePipe = true;
		
		fprintf(stdout, "RESULTS IS: %d\n", results);
		
		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "check for opposite request succesfully\n");
		}

		if(callback_return.length() > 0){

			printf("opposite request exists that is actually from user %s to user %s\n", otherUserKey, key);			


			printf("atoi return from %s = %d\n", callback_return.c_str(), atoi(callback_return.c_str()));
			
			if(atoi(callback_return.c_str()) == 1){
				printf("and that does = 1, so it has been accepted, but not acknowledged by the requesting user\n");	
				
				printf("must extract id %s from the current user's friends list\n", otherUserKey);
			// extract	
				query = "SELECT friendsList USERS WHERE id="+string(key)+";";
		
				fprintf(stdout, "%s\n", query.c_str());

				resetCallback_return();

				usePipe = false;
				results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
				usePipe = true;
						
				fprintf(stdout, "RESULTS IS: %d\n", results);
		
				if(results != SQLITE_OK){
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
				} else {
					fprintf(stdout, "extracted friendsList succesfully\n");
				}

////////

				callSave = callback_return.c_str();

				printf("friendsList for user %s = %s\n", key, callSave.c_str());

				ret = stringFindLocation(callSave.c_str(), otherUserKey);

				if(ret < 0){
					// shouldn't unjoin because the alias isn't in
					printf("DELETE FRIEND ERROR: Key %s was not friends with user %s\n", otherUserKey, key);			
					write(slaveSocket, "INVALID",7);
					printf("Wrote back INVALID\n");
					sqlite3_close(db);
					dbOpen = false;
					return;
				}

				std::string newFriendsList;
				newFriendsList = callSave.substr(0,ret) + callSave.substr(ret+keyL);
		
				printf("New friendsList after extracting key %s at index %d = %s\n", otherUserKey, ret, newFriendsList.c_str()); 

				query = "UPDATE USERS SET friendsList ='"+string(newFriendsList.c_str())+"' WHERE id="+string(key)+";";
		
				results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);

				if(results != SQLITE_OK){
					fprintf(stderr, "SQL error: %s\n", zErrMsg);
				} else {
					fprintf(stdout, "updated friendsList succesfully, boner!\n");
				}				
				
			}
			
			// delete the exact request

			query = "UPDATE FRIENDREQUESTS SET results=-1 WHERE requestingUser="+string(otherUserKey)+" AND targetUser="+string(key)+";";

			fprintf(stdout, "%s\n", query.c_str());

			resetCallback_return();
		
			results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		
			fprintf(stdout, "RESULTS IS: %d\n", results);
		
			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "opposite request REJECTED succesfully, biatch!\n");
			}

			sqlite3_close(db);
			dbOpen = false;
				
			write(slaveSocket,"ISVALID",7);
			printf("Wrote back ISVALID\n");
			
			return;
		}

		// neither request exists
		printf("Neither request existed\n");

		printf("must extract id %s from the current user's friends list\n", otherUserKey);
	// extract	
		query = "SELECT friendsList USERS WHERE id="+string(key)+";";
		
		fprintf(stdout, "%s\n", query.c_str());

		resetCallback_return();

		usePipe = false;
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		usePipe = true;
						
		fprintf(stdout, "RESULTS IS: %d\n", results);
		
		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "extracted friendsList succesfully\n");
		}

////////

		callSave = callback_return.c_str();

		printf("friendsList for user %s = %s\n", key, callSave.c_str());

		ret = stringFindLocation(callSave.c_str(), otherUserKey);

		if(ret < 0){
			// shouldn't unjoin because the alias isn't in
			printf("DELETE FRIEND ERROR: Key %s was not friends with user %s\n", otherUserKey, key);			
			write(slaveSocket, "INVALID",7);
			printf("Wrote back INVALID\n");
			sqlite3_close(db);
			dbOpen = false;
			return;
		}

		std::string newFriendsList;
		newFriendsList = callSave.substr(0,ret) + callSave.substr(ret+keyL);
	
		printf("New friendsList after extracting key %s at index %d = %s\n", otherUserKey, ret, newFriendsList.c_str()); 

		query = "UPDATE USERS SET friendsList ='"+string(newFriendsList.c_str())+"' WHERE id="+string(key)+";";
	
		results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "updated friendsList succesfully, motherfucker!\n");
		}	


		// now also need to add a delete request to the FRIENDREQUESTS table

		query = "INSERT INTO FROM FRIENDREQUESTS (requestingUser, targetUser, results) VALUES("+string(key)+","+string(otherUserKey)+",-2);";
		
		fprintf(stdout, "%s\n", query.c_str());

		resetCallback_return();
		
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		
		fprintf(stdout, "RESULTS IS: %d\n", results);
		
		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "inserted delete friend request succesfully\n");
		}
						
		sqlite3_close(db);
		dbOpen = false;	
		
		write(slaveSocket,"ISVALID",7);
		printf("Wrote back ISVALID\n");
		//database stuff
	}
	else
	{
		write(slaveSocket,"INVALID",7);
		printf("Wrote back INVALID\n");
	}	
	
}




//////////////////////////////////////////////////////////////////////////////   Internal uses   //////////////////////////////////////////////////////////////////////////////
 
//INPUT:	A string key and password.
//IDEA:		Accept a key and password pair and check if they match.
//OUTPUT:	NONE
//RETURN:	True if pair match, false if they do not.
bool isGoodPass(char * key, char * password)
{


		//sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;
			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		const char* data = "Callback function called";


		resetCallback_return();
	
	
		query = "SELECT password FROM USERS WHERE id="+string(key)+";";
		
		
fprintf(stdout, "%s\n", query.c_str());

		resetCallback_return();
		
		usePipe = false;
		results = sqlite3_exec(db, query.c_str(), callback, (void*) data , &zErrMsg);
		usePipe = true;

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "is good pass\n");
		}
		
		//sqlite3_close(db);
		//dbOpen = false;

		if(strncmp(callback_return.c_str(),password,21) == 0) //need to add in a reference to the password that the provided key matches ************************ 
		{
			fprintf(stdout, "password is correct\n");
			return true;
		}
		fprintf(stdout, "password is incorrect\n");
		return false;


		

	

}

//INPUT:	String to be checked. 	ex: Location, Date, time, skill, or sport.
//IDEA:		Check if the provided string contains actual information.
//OUTPUT:	NONE
//RETURN:	Check if input contains important information. If it does return True, if not return false.
bool isValidValue(char * string)
{
	if(string == NULL || string[0] == ' ')
	{
		fprintf(stdout, "value is not valid yo\n");
		return false;
	}

	if(!isSwearFree(string)){
		return false;
	}

	return true;
}

bool isSwearFree(char * string)
{
	int i;

	for(i = 0; i < numBad; i++){

		if(strcasestr(string, bad[i]) != NULL)
		{
			fprintf(stdout, "Curse words! %i: %s|%s\n",i,string,bad[i]);
			return false;
		}
	}

	return true;

}

bool isCorrectUser(char * key, char * evKey)
{

		//sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		const char* data = "Callback function called";

		resetCallback_return();	

		query = "SELECT creatingUser FROM EVENTS WHERE id="+string(evKey)+";";
fprintf(stdout, "%s\n", query.c_str());

		resetCallback_return();
		
		usePipe = false;
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		usePipe = true;

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);

		} else {
			fprintf(stdout, "User checked\n");
		}
		
		//sqlite3_close(db);
		//dbOpen = false;


		if(strncmp(callback_return.c_str(),key,5) == 0)  
		{
			return true;
		}
		return false;
		

}

bool isGoodSet( char * alias,char * password)
{
	
		//sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		const char* data = "Callback function called";

		resetCallback_return();	

		query = "SELECT password FROM USERS WHERE alias="+string(alias)+";";
fprintf(stdout, "%s\n", query.c_str());

		resetCallback_return();
		
		usePipe = false;
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		usePipe = true;

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "is good set\n");
		}
		
		//sqlite3_close(db);
		//dbOpen = false;

		if(strncmp(callback_return.c_str(),password,21) == 0) 
		{
			return true;
		}
		return false;


		
}

bool checkAlias(char * alias)
{

		//sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		const char* data = "Callback function called";

		resetCallback_return();	

		query = "SELECT alias FROM USERS WHERE alias='"+string(alias)+"';";
fprintf(stdout, "%s\n", query.c_str());

		resetCallback_return();
		usePipe = false;
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		usePipe = true;  

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "alias checked\n");
		}
		
		//sqlite3_close(db);
		//dbOpen = false;
	
	if(strncmp(callback_return.c_str(),alias,21) == 0)
	{
		fprintf(stdout, "alias is taken\n");
		return true;
	}
	else
	{
		fprintf(stdout, "alias isn't taken\n");
		return false;
	}
}

void resetCallback_return()
{
	callback_return = "";
}

void getAlias(char * key, char * returnPlace)
{
	char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;

		string saveCall;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		const char* data = "Callback function called";

		query = "SELECT alias FROM USERS WHERE id="+string(key)+";";
fprintf(stdout, "%s\n", query.c_str());
		
		saveCall = callback_return.c_str();

		resetCallback_return();

		usePipe = false;
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		usePipe = true;

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "alias converted from key\n");
		}
		
		strcpy(returnPlace, callback_return.c_str());

		callback_return = saveCall.c_str();
}

int stringFindLocation(const char * input, char * key)
{
		//return 0 =< infinity
		//-1 if its not in the string
		char tester[50];
		strcpy(tester, key);
		strcat(tester, "*");
		
		char * result;
		result= strstr((char *)input,tester);
		if(result != NULL)
		{
			return (int)(result - input);
		} 
		else
		{
			return -1;
		}
}

void lockRow(char *evKey, char *userKey){

		char *zErrMsg = 0;
		int rc;
		std::string query;
		std::string query2;
		int results;
		char* user;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		const char* data = "Callback function called";
		
		int flag = 1;
		
		query = "UPDATE EVENTS SET owningUser="+string(userKey)+" WHERE id="+string(evKey)+" AND owningUser=0;";
		query2 = "SELECT owningUser FROM EVENTS WHERE id="+string(evKey)+";";
		
		while(flag){
		
			results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);

			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "set owningUser\n");
			}

			resetCallback_return();
			
			usePipe = false;
			results = sqlite3_exec(db, query2.c_str(), callback, (void *) data, &zErrMsg);
			usePipe = true;
			
			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "owningUser checked\n");
			}
			printf("STRING!:%s = %s\n", callback_return.c_str(), userKey);
			if(atoi(callback_return.c_str())== atoi(userKey)){
				flag = 0;
			}
		
		}
		
	
	// returns 0 if the row belongs to someone else. returns 1 if the row belongs to you
	
}

void unlockRow(char * evKey){
	
		char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		const char* data = "Callback function called";

		resetCallback_return();	

		query = "UPDATE EVENTS SET owningUser=0 WHERE id="+string(evKey)+";";

		results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "Row unlocked\n");
		}
	
	
}

void getAliases(int slaveSocket, char * idList)
{
	aliasesFromIDs(idList);
	write(slaveSocket,callback_return.c_str(), callback_return.length());
}

char * aliasesFromIDs(char * idList )
{
	string s = string(idList);
	replace(s.begin(), s.end(), '*', ',');
	s = s.substr(0,s.size()-1);

		char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		const char* data = "Callback function called";

		callback2 = callback_return.c_str();

		resetCallback_return();

		query = "SELECT alias FROM USERS WHERE id in ("+s+");";

		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "aliases converted\n");
		}

		return (char *)callback_return.c_str();
	
}

void getKeyFromAlias(char * alias, char * returnPlace){
		
		char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;

		if(!dbOpen){
			rc = sqlite3_open("serverDatabase.db", &db);
			dbOpen = true;

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}
		}

		const char* data = "Callback function called";

		resetCallback_return();	

		query = "SELECT id FROM USERS WHERE alias='"+string(alias)+"';";
		fprintf(stdout, "%s\n", query.c_str());

		resetCallback_return();
		usePipe = false;
		results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);
		usePipe = true;

		if(results != SQLITE_OK){
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
		} else {
			fprintf(stdout, "key %s converted from alias %s\n", callback_return.c_str(), alias);
		}
		
		strcpy(returnPlace, callback_return.c_str());	
	
}
