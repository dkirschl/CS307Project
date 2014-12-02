#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include <iostream>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sqlite3.h>
#include <algorithm>

const int sportL = 31;
const int locL = 31;
const int dateL = 9;
const int timeL = 5;
const int summL = 101;
const int passL = 21;
const int nameL = 51;
const int aliasL = 21;
const int ageL = 3;
const int genderL = 2;
const int descL = 101;
const int keyL = 5;
const int evKeyL = 5;
const int competeL = 2;
const int titleL = 26;
const int attendL = 4;
const int paramL = 2;

void work(int);


//external calls

//INPUT: 	slaveSocket, password, name, alias, age, gender, XXXdescriptionXXXX
//IDEA:		Accept input and create a user based on provided info.
//OUTPUT:	Write confirmation back to user if successful.
//RETURN:	NONE
void dataCreateUser(int,char *, char *, char *, char *, char *); 

//INPUT:	slaveSocket, key, password, sport, location, date, time, summary, skill level
//IDEA:		Accept into and create an event based on provided info.
//OUTPUT:	Write confirmation back to user if successful.
//RETURN:	NONE
void dataCreateEvent(int,char *,char *,char *,char *,char *,char *,char *,char *,char *, char *);

//INPUT:	slaveSocket, key, password, name, alias, age, gender, desc.
//IDEA:		Accept info, check if it is valid. If valid change relevent user data.
//OUTPUT:	Confirmation back if successful.
//RETURN:	NONE
void dataUpdateUser(int,char *,char *,char *,char *,char *,char *,char *,char *);

//INPUT:	slaveSocket, key, password, event key, sport, location, date, time, summary, skill level
//IDEA:		Accept info, check if it is valid. If valid, change relevent event data.
//OUTPUT:	Confirmation back if successful.
//RETURN:	NONE
void dataUpdateEvent(int,char *,char *,char *,char *,char *,char *,char *,char *,char *,char *, char *);

//INPUT:	Key, password, Event key
//IDEA:		Accept info, check if valid. If valid, delete specified event.
//OUTPUT:	Confirmation of success or failure.
//RETURN:	NONE
void dataDeleteEvent(int,char *,char *,char *);

//INPUT:	Sport, location, date, time, skill level
//IDEA:		Accept info, check for valid inputs. For valid inputs search the database.  
//OUTPUT:	Events matching request.
//RETURN:	NONE
void dataGetEvent(int,char *,char *,char *,char *,char *, char *);

void dataDeleteUser(int, char *, char *);

void dataJoinEvent(int, char *, char *,char *, int);

void dataUnJoinEvent(int, char *, char *, char *);

void dataLogOn(int, char *, char *);//????

void updateTopSports(int, char *, char *, char *, char *, char *);

void getCreatedNumAttending(int, char *);

void dataCheck(int, char *);

void dataAddFriend(int, char *, char *, char *);

void dataDeleteFriend(int, char *, char *, char *);

//Internal uses

//INPUT:	A string key and password.
//IDEA:		Accept a key and password pair and check if they match.
//OUTPUT:	NONE
//RETURN:	True if pair match, false if they do not.
bool isGoodPass(char *, char *);

//INPUT:	String to be checked. 	ex: Location, Date, time, skill, or sport.
//IDEA:		Check if the provided string contains actual information.
//OUTPUT:	NONE
//RETURN:	Check if input contains important information. If it does return True, if not return false.
bool isValidValue(char *);

bool isGoodSet( char * ,char * );

void resetCallback_return(void);

bool checkAlias(char *);

void unlockRow(char *);
void lockRow(char *, char *);

//check if a given event is owned by a given user
bool isCorrectUser(char *, char *);

int stringFindLocation(const char *, char *);
void getAlias(char *, char *);

void getKeyFromAlias(char *, char *);

char * aliasesFromIDs(char *);

int createDatabases();

extern sqlite3 *db;
extern bool dbOpen;
