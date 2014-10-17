//DATABASE INTERFACE CODE

#include "tbinclude.h"
#include <sqlite3.h>
using namespace std;

string callback_return;


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
	if(argc != 1){
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
	if(isValidValue(password) && isValidValue(name) && isValidValue(alias) && isValidValue(age) && isValidValue(gender))
	{
		
		//add user to the database and return the key from the database if there is an error then return -1
		write(slaveSocket,"ISVALID",7);


		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		std::string query;
		std::string query2;
		int results;
		int file_entry;
		char* user;

		rc = sqlite3_open("serverDatabase.db", &db);

		if(rc){
			fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
		} else {
			fprintf(stderr, "Opened database succesfully\n");
		}

		// check if alias is unique
		//

		const char* data = "Callback functioin called";

		query = "INSERT INTO USERS (password, name, alias, age, gender) VALUES('"+string(password)+"','"+ string(name) + "','" + string(alias) + "','" + string(age) + "','" + string(gender) + "');";
		query2 = "SELECT seq FROM sqlite_sequence WHERE name='USERS';";
		//query2 = "SELECT last_row_id();";
fprintf(stdout, "%s\n", query.c_str());
		results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);
fprintf(stdout, "RESULTS IS: %d\n", results);
		file_entry = sqlite3_exec(db, query2.c_str(), callback, (void*) data, &zErrMsg);

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
void dataCreateEvent(int slaveSocket,char * key,char * password,char * sport,char * location,char * date,char * time,char * summary,char * skill)
{
	if(isValidValue(password) && isValidValue(sport) && isValidValue(location) && isValidValue(date) && isValidValue(time) && isValidValue(summary) && isValidValue(skill))
	{
		if(isGoodPass(key, password) == false)
		{
			write(slaveSocket,"INVALID",7);
		}
		else
		{
		
		//Do stuff ************************

			write(slaveSocket,"ISVALID",7);

			sqlite3 *db;
			char *zErrMsg = 0;
			char *zErrMsg2 = 0;
			int rc;
			std::string query;
			std::string query2;
			int results;
			int file_entry;
			char* event;

			rc = sqlite3_open("serverDatabase.db", &db);

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}

			const char* data = "Callback functioin called";

			query = "INSERT INTO EVENTS (sport, location, date, time, summary,desiredSkillLevel) VALUES('"+string(sport)+"','"+ string(location) + "','" + string(date) + "','" + string(time) + "','" + string(summary) + "','" + string(skill) + "');";
			query2 = "SELECT seq FROM sqlite_sequence WHERE name='EVENTS';";
			//query2 = "SELECT last_row_id();";
	fprintf(stdout, "%s\n", query.c_str());
			results = sqlite3_exec(db, query.c_str(), callback, 0, &zErrMsg);
	fprintf(stdout, "RESULTS IS: %d\n", results);
			file_entry = sqlite3_exec(db, query2.c_str(), callback, (void*) data, &zErrMsg2);

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
void dataUpdateUser(int slaveSocket,char * key,char * password,char * name,char * alias,char * age,char * gender,char * desc)
{
	if(isValidValue(password) && isValidValue(name) && isValidValue(alias) && isValidValue(age) && isValidValue(gender) && isValidValue(desc))
	{
		if(isGoodPass(key, password) == false)
		{
			write(slaveSocket,"INVALID",7);
		}
		else
		{
		
		//Do stuff ************************

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
void dataUpdateEvent(int slaveSocket,char * key,char * password,char * evKey,char * sport,char * location,char * date,char * time,char * summary,char * skill)
{
	if(isValidValue(password) && isValidValue(sport) && isValidValue(location) && isValidValue(date) && isValidValue(time) && isValidValue(summary) && isValidValue(skill))
	{
		if(isGoodPass(key, password) == false || isCorrectUser(key,evKey) == false)
		{
			write(slaveSocket,"INVALID",7);
		}
		else
		{
		
		//Do stuff ************************

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
			
			sqlite3 *db;
			char *zErrMsg = 0;
			int rc;
			std::string query;
			int results;
			int file_entry;

			rc = sqlite3_open("serverDatabase.db", &db);

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
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
			fprintf(stdout, "Finished action\n");

////////////////////////////////////////////////////////////////		
		
	}
}

//INPUT:	Sport, location, date, time, skill level
//IDEA:		Accept info, check for valid inputs. For valid inputs search the database.  
//OUTPUT:	Events matching request.
//RETURN:	NONE
void dataGetEvent(int slaveSocket,char * sport,char * location,char * date,char * time,char * skill)
{

	write(slaveSocket,"ISVALID",7);


	string query = "SELECT summary, date FROM EVENTS";
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
		query += " location ='" + string(date) + "' ";
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
		query += " location ='" + string(time) + "' ";
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
		query += " location ='" + string(skill) + "' ";
		flag = 1;
	}

	query += ";";

///////////////////////////////////////////////////////////////////////

			sqlite3 *db;
			char *zErrMsg = 0;
			int rc;
			int results;
			int file_entry;

			rc = sqlite3_open("serverDatabase.db", &db);

			if(rc){
				fprintf(stderr, "Unable to open database: %s\n", sqlite3_errmsg(db));
			} else {
				fprintf(stderr, "Opened database succesfully\n");
			}

			const char* data = "Callback function called";

			fprintf(stdout, "%s\n", query.c_str());
			callback_return = "|";
			results = sqlite3_exec(db, query.c_str(), callback, (void*) data, &zErrMsg);

			if(results != SQLITE_OK){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
			} else {
				fprintf(stdout, "Event inserted succesfully\n");
			}
		
			fprintf(stdout, "callback_return: %s\n", callback_return.c_str());
			sqlite3_close(db);
			write(slaveSocket, callback_return.c_str(), callback_return.length());
			fprintf(stdout, "Wrote back\n");
	
///////////////////////////////////////////////////////////////////////////////////////	


}

//Internal uses

//INPUT:	A string key and password.
//IDEA:		Accept a key and password pair and check if they match.
//OUTPUT:	NONE
//RETURN:	True if pair match, false if they do not.
bool isGoodPass(char * key, char * password)
{
	int *temp;
	int value;
	temp = (int *) key;
	value = *temp;

	printf("\n value = %d\n", value);

	// SELECT password FROM users WHERE id = value
	/*
	if(strncmp(,password,21) == 0) //need to add in a reference to the password that the provided key matches ************************ 
	{
		return true;
	}
	return false;
	*/

	return true;
}

//INPUT:	String to be checked. 	ex: Location, Date, time, skill, or sport.
//IDEA:		Check if the provided string contains actual information.
//OUTPUT:	NONE
//RETURN:	Check if input contains important information. If it does return True, if not return false.
bool isValidValue(char * string)
{
	if(string == NULL || string[0] == ' ')
	{
		return false;
	}
	return true;
}

bool isCorrectUser(char * key, char * evKey)
{
	if(1)	//event owner key == given key
	{
		return true;
	}
	else
	{
		return false;
	}
}
