//DATABASE INTERFACE CODE

#include "tbinclude.h"

//INPUT: 	slaveSocket, password, name, alias, age, gender, description
//IDEA:		Accept input and create a user based on provided info.
//OUTPUT:	Write confirmation back to user if successful.
//RETURN:	NONE
void dataCreateUser(int slaveSocket,char * password, char * name, char * alias, char * age, char * gender, char * desc)
{
	if(isValidValue(password) && isValidValue(name) && isValidValue(alias) && isValidValue(age) && isValidValue(gender) && isValidValue(desc))
	{
		//Do stuff ************************
		//add user to the database and return the key from the database if there is an error then return -1
		write(slaveSocket,"ISVALID",7);
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
	
	//Do stuff ************************

		write(slaveSocket,"ISVALID",7);
	}
}

//INPUT:	Sport, location, date, time, skill level
//IDEA:		Accept info, check for valid inputs. For valid inputs search the database.  
//OUTPUT:	Events matching request.
//RETURN:	NONE
void dataGetEvent(int slaveSocket,char * sport,char * location,char * date,char * time,char * skill)
{
	if(isValidValue(sport) && isValidValue(location) && isValidValue(date) && isValidValue(time) && isValidValue(skill))
	{	
		//Do stuff ************************

			write(slaveSocket,"ISVALID",7);
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
	if(strncmp(,password,21) == 0) //need to add in a reference to the password that the provided key matches ************************ 
	{
		return true;
	}
	return false;
}

//INPUT:	String to be checked. 	ex: Location, Date, time, skill, or sport.
//IDEA:		Check if the provided string contains actual information.
//OUTPUT:	NONE
//RETURN:	Check if input contains important information. If it does return True, if not return false.
bool isValueValid(char * string)
{
	if(string == NULL || string[0] == ' ')
	{
		return false;
	}
	return true;
}

bool isCorrectUser(char * key, char * evKey)
{
	if()	//event owner key == given key
	{
		return true;
	}
	else
	{
		return false;
	}
}
