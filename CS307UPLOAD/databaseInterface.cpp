//DATABASE INTERFACE CODE

#include "tbinclude.h"
using namespace std;

string callback_return;

sqlite3 *db;
bool dbOpen = false;
bool usePipe = true;

static int callback(void *data, int argc, char **argv, char **azColName){
	int i;
	fprintf(stdout, "%s: ", (const char*)data);
	fprintf(stdout, "%d\n", argc);
	for(i = 0; i<argc; i++){
		printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
		callback_return += string(argv[i]);
		if(i != (argc - 1)){
			callback_return += "~";
		}
	}
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

		query = "INSERT INTO USERS (password, name, alias, age, gender, firstSport, secondSport, thirdSport) VALUES('"+string(password)+"','"+ string(name) + "','" + string(alias) + "','" + string(age) + "','" + string(gender) + "','  ','  ','  ');";
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
	}
}



void dataDeleteUser(int slaveSocket, char * key, char * password)
{
	if(isValidValue(password) && isGoodPass(key, password))
	{
		write(slaveSocket,"ISVALID",7);
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
	}
}

void dataJoinEvent(int slaveSocket, char * key, char * password, char * evKey)
{
	if(isValidValue(password) && isGoodPass(key, password) && isValidValue(evKey))
	{
	
		char *zErrMsg = 0;
		int rc;
		std::string query;
		int results;
		char* user;
		char alias[21];
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

		getAlias(key, alias);
		
		lockRow(evKey,key);

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

		ret = stringFindLocation(callSave.c_str(), alias);

		fprintf(stdout, "User Id: %s\n", callback_return.c_str());

		if(ret >= 0){
			// shouldn't join because the alias is already in
			write(slaveSocket, "INVALID",7);
			unlockRow(evKey);
			sqlite3_close(db);
			dbOpen = false;
			return;
		}

		callSave += string(alias) + "|";
		
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
		
		unlockRow(evKey);
		
		sqlite3_close(db);
		dbOpen = false;

		write(slaveSocket, callback_return.c_str(), callback_return.length());
		fprintf(stdout, "Wrote back\n");
		
		
		write(slaveSocket,"ISVALID",7);
		//database stuff
	}
	else
	{
		write(slaveSocket,"INVALID",7);
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
		char alias[21];
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

		getAlias(key, alias);
		
		lockRow(evKey,key);

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

		ret = stringFindLocation(callSave.c_str(), alias);

		if(ret < 0){
			// shouldn't join because the alias is already in
			write(slaveSocket, "INVALID",7);
			unlockRow(evKey);
			sqlite3_close(db);
			dbOpen = false;
			return;
		}

		std::string newAttend;
		newAttend = callSave.substr(0,ret) + callSave.substr(ret+aliasL);
		
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
		
		unlockRow(evKey);
		
		sqlite3_close(db);
		dbOpen = false;

		write(slaveSocket, callback_return.c_str(), callback_return.length());
		fprintf(stdout, "Wrote back\n");
				
		
		
		write(slaveSocket,"ISVALID",7);
		//database stuff
	}
	else
	{
		write(slaveSocket,"INVALID",7);
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
		}
		else
		{
		
		//Do stuff ************************

			write(slaveSocket,"ISVALID",7);

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

			query = "INSERT INTO EVENTS (sport, location, date, time, summary,desiredSkillLevel, creatingUser, title, numAttending, maxNumAttending, owningUser) VALUES('"+string(sport)+"','"+ string(location) + "','" + string(date) + "','" + string(time) + "','" + string(summary) + "','" + string(compete) + "'," + string(key)+",'"+string(title)+"','"+string("0")+"','"+string(attendM)+"',0);";

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

			write(slaveSocket, callback_return.c_str(), callback_return.length());
			fprintf(stdout, "Wrote back\n");
	
			
		}
	}
	else
	{
		write(slaveSocket,"INVALID",7);
	}
}

//INPUT:	slaveSocket, key, password, name, alias, age, gender, desc.
//IDEA:		Accept info, check if it is valid. If valid change relevent user data.
//OUTPUT:	Confirmation back if successful.
//RETURN:	NONE
void dataUpdateUser(int slaveSocket,char * key,char * password,char * name,char * alias,char * age,char * gender,char * desc, char * newPass)
{
	if(isValidValue(password) && isValidValue(name) && isValidValue(alias) && isValidValue(age) && isValidValue(gender) && isValidValue(desc) && isValidValue(newPass))
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
			results = sqlite3_exec(db, query.c_str(), callback, (void *) data, &zErrMsg);

			if(strncmp(callback_return.c_str(), alias, 21) == 0){
				aliasFlag = 0;
			}

			if(aliasFlag == 1){
				if(checkAlias(alias)){
					write(slaveSocket, "INVALID ALIAS", 13);
					sqlite3_close(db);
					return;
				}
			}


////////////////////////////////////
			query = "UPDATE USERS SET name='"+string(name)+"' alias='"+string(alias)+"' age='"+string(age)+"' gender='"+string(gender)+"' description='"+string(desc)+"' password='"+ string(newPass) +"' WHERE id="+string(key)+";"; 
			
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
		}
	}
	else
	{
		write(slaveSocket,"INVALID",7);
	}
}

//INPUT:	slaveSocket, key, password, event key, sport, location, date, time, summary, skill level
//IDEA:		Accept info, check if it is valid. If valid, change relevent event data.
//OUTPUT:	Confirmation back if successful.
//RETURN:	NONE
void dataUpdateEvent(int slaveSocket,char * key,char * password,char * evKey,char * sport,char * location,char * date,char * time,char * summary,char * skill, char * title, char * attendM)
{
	if(isValidValue(password) && isValidValue(sport) && isValidValue(location) && isValidValue(date) && isValidValue(time) && isValidValue(summary) && isValidValue(skill) && isValidValue(title) && isValidValue(attendM))
	{
		if(isGoodPass(key, password) == false || isCorrectUser(key,evKey) == false)
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
		}
	}
	else
	{
		write(slaveSocket,"INVALID",7);
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
	}
	else
	{

		write(slaveSocket,"ISVALID",7);

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
void dataGetEvent(int slaveSocket,char * sport,char * location,char * date,char * time,char * skill, char * title)
{

	write(slaveSocket,"ISVALID",7);


	string query = "SELECT title, date FROM EVENTS";	//need to return more
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
		query += " date ='" + string(date) + "' ";
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
		query += " time ='" + string(time) + "' ";
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

			results = sqlite3_exec(db, query.c_str(), callback, (void*) data, &zErrMsg);

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
		}
	}
	else
	{
		write(slaveSocket,"INVALID",7);
	}
}

//Internal uses

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

		query = "SELECT alias FROM USERS WHERE id="+string(key)+";";
fprintf(stdout, "%s\n", query.c_str());

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
}

int stringFindLocation(const char * input, char * alias)
{
		//return 0 =< infinity
		//-1 if its not in the string
		char tester[50];
		strcpy(tester, alias);
		strcpy(tester, "|");
		
		char * result;
		result= strstr((char *)input,alias + "|");
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
		
		query = "UPDATE EVENTS SET owningUser= "+string(userKey)+" WHERE id="+string(evKey)+" AND owningUser=0;";
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
			
			if(strcmp(callback_return.c_str(), key) == 0){
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
