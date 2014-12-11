//Message handler By Dylan Smith
#include "tbinclude.h"

void crus(int); //create user
void deus(int); //delete user
void crev(int);// create event
void upus(int); //update user
void upev(int);// //update event
void deev(int); //delete event
void gtev(int); //get events
void invl(int); //invalid
void join(int);//join event
void unjn(int); //unjoin event
void lgon(int); //logon
void upts(int); //update top 3 sports
void crup(int);	//
void chck(int);	//check
void addf(int);	//add friend
void delf(int);	//delete Friend

void gtfq(int);	//get friend requests
void gtgm(int);	//get game
void gtal(int);	//getall
void ingm(int); //invite to game
void dlgi(int); //delete game invite
void dlfq(int); //delete friend request
void gail(int);
void trns(int);


void printStandard(int,char *);
void readRemainder(int);
void readStuffs(int, char *, int);

void work(int slaveSocket)
{
	/* How recieved messages will appear:

				POST <sp> /<script>?{<var>=<val>&}*{<var>=<val>}<sp> HTTP/1.0 <crlf>
   				{<Other Header Information> <crlf>}*
    			<crlf>
	
	//Current:
		crev: (create event)
	GET<sp>/crev/sport/location/date/time/summary/user/wantedskill/<sp>HTTP/1.0 <crlf>
	
		crus: (create user)
	GET<sp>/crus/NAME/ALIAS/AGE/GENDER/PASSWORD/<sp>HTTP/1.0 <crlf>

		upus: (update user)
	GET<sp>/upus/KEY/PASS/NAME/ALIAS/AGE/GENDER/DESC/<sp>HTTP/1.0 <crlf>

		upev: (update event)
	GET<sp>/upev/KEY/PASS/EVKEY/SPORT/LOC/DATE/TIME/SUMM/SKILL/<sp>HTTP/1.0 <crlf>

		deev: (delte event)
	GET<sp>/deev/KEY/PASS/EVKEY/<sp>HTTP/1.0 <crlf>

		gtev: (get events)
	GET<sp>/gtev/SPORT/LOC/DATE/TIME/SKILL/<sp>HTTP/1.0 <crlf>



		//example
		POST<sp>/see-DAY-TIME- HTTP/1.0 <crlf>
		{other <crlf>}*
		<crlf>

		POST<sp>/add-DAY-TIME-NAME-CLASS-ROOM- HTTP/1.0<crlf>
		{other <crlf>}*
		<crlf>

	*/

	char input1 [10];
	unsigned char next;
	unsigned char last = 0;
	int tempi;

	printf("working attempt");
	
	//read in get
	for(tempi = 0; tempi < 4; tempi++)
	{
		next = read(slaveSocket, &next, sizeof(next));
	}

	for(tempi = 0; tempi < 5; tempi++)
	{
		read(slaveSocket, &next, sizeof(next));
		input1[tempi] = next;
	}
	input1[tempi] = '\0';

	printf("%s\n",input1);
	
	if(strcmp(input1,"/crus\0") == 0)			//CREATE USER
	{
		crus(slaveSocket);
	}
	else if(strcmp(input1,"/deus\0") == 0) 		
	{
		deus(slaveSocket);
	}	
	else if(strcmp(input1,"/crev\0") == 0)		//CREATE EVENT
	{
		crev(slaveSocket);
	}
	else if(strcmp(input1,"/upus\0") == 0)		//UPDATE USER
	{
		upus(slaveSocket);
	}
	else if(strcmp(input1,"/upev\0") == 0)		//UPDATE EVENT
	{
		upev(slaveSocket);
	}
	else if(strcmp(input1,"/deev\0") == 0) 		//DELETE EVENT
	{
		deev(slaveSocket);
	}
	else if(strcmp(input1,"/gtev\0") == 0)		//GET EVENT
	{
		gtev(slaveSocket);

	} 
	else if(strcmp(input1,"/join") == 0)	//join event
	{
		join(slaveSocket);
	}
	else if(strcmp(input1,"/unjn") == 0) //unjoin event
	{
		unjn(slaveSocket);
	}
	else if(strcmp(input1,"/lgon") == 0)	//log into the server using alias and password
	{
		lgon(slaveSocket);
	}
	else if(strcmp(input1,"/upts") == 0)
	{
		upts(slaveSocket);					// update top sports for a user
	}
	else if(strcmp(input1,"/crup") == 0)
	{
		crup(slaveSocket);				
	}
	else if(strcmp(input1,"/chck") == 0)
	{
		chck(slaveSocket);
	}
	else if(strcmp(input1,"/addf") == 0)
	{
		addf(slaveSocket);
	}
	else if(strcmp(input1,"/delf") == 0)
	{
		delf(slaveSocket);
	}
	else if(strcmp(input1,"/gtfq") == 0)
	{
		gtfq(slaveSocket);
	}	
	else if(strcmp(input1,"/gtgm") == 0)
	{
		gtgm(slaveSocket);
	}	
	else if(strcmp(input1,"/gtal") == 0)
	{
		gtal(slaveSocket);
	}
	else if(strcmp(input1,"/ingm") == 0)
	{
		ingm(slaveSocket);
	}
	else if(strcmp(input1,"/dlgi") == 0)
	{
		dlgi(slaveSocket);
	}
	else if(strcmp(input1,"/dlfq") == 0)
	{
		dlfq(slaveSocket);
	}
	else if(strcmp(input1,"/gail") == 0)
	{
		gail(slaveSocket);
	}
	else 										//INVALID INPUT
	{
		invl(slaveSocket);
	}
}


////////////////////////////////


void gail(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[1001];	//my key 


	//GET INPUT
	printf("Hey");

	readStuffs(slaveSocket, input2, 1001);
	
	read(slaveSocket, &next, sizeof(next));
	printf("My hands are typing words lsdf;laksdjf;l.\n");
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/gail";
	printStandard(slaveSocket,stringA);
	//database code
	getAliases(slaveSocket,input2);

	close(slaveSocket);
}

void dlfq(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];	//my key
	char input3[passL];	//pass 
	char input4[keyL];	//thier key


	//GET INPUT
	printf("Hey");

	readStuffs(slaveSocket, input2, keyL);
	readStuffs(slaveSocket, input3, passL);
	readStuffs(slaveSocket, input4, keyL);
	
	read(slaveSocket, &next, sizeof(next));
	printf("My hands are typing words lsdf;laksdjf;l.\n");
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/dlfq";
	printStandard(slaveSocket,stringA);
	//database code
	deleteFriendRequest(slaveSocket,input2, input3, input4);

	close(slaveSocket);
}

void dlgi(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];	//PASS
	char input3[passL];	//NAME
	char input4[evKeyL];


	//GET INPUT
	printf("Hey");

	readStuffs(slaveSocket, input2, keyL);
	readStuffs(slaveSocket, input3, passL);
	readStuffs(slaveSocket, input4, evKeyL);
	
	read(slaveSocket, &next, sizeof(next));
	printf("My hands are typing words lsdf;laksdjf;l.\n");
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/dlgi";
	printStandard(slaveSocket,stringA);
	//database code
	deleteGameInvite(slaveSocket,input2, input3, input4);

	close(slaveSocket);
}

void ingm(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];	//PASS
	char input3[passL];	//NAME
	char input4[keyL];
	char input5[evKeyL];


	//GET INPUT
	printf("Hey");

	readStuffs(slaveSocket, input2, keyL);
	readStuffs(slaveSocket, input3, passL);
	readStuffs(slaveSocket, input4, keyL);
	readStuffs(slaveSocket, input5, evKeyL);
	
	read(slaveSocket, &next, sizeof(next));
	printf("My hands are typing words lsdf;laksdjf;l.\n");
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/ingm";
	printStandard(slaveSocket,stringA);
	//database code
	dataInviteGame(slaveSocket,input2, input3, input4, input5);

	close(slaveSocket);
}

void gtfq(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];	//PASS
	char input3[passL];	//NAME


	//GET INPUT
	printf("Hey");

	readStuffs(slaveSocket, input2, keyL);
	readStuffs(slaveSocket, input3, passL);
	
	read(slaveSocket, &next, sizeof(next));
	printf("My hands are typing words lsdf;laksdjf;l.\n");
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/gtfq";
	printStandard(slaveSocket,stringA);
	//database code
	dataGetFR(slaveSocket,input2, input3);

	close(slaveSocket);
}



void gtgm(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];	//PASS
	char input3[passL];	//NAME


	//GET INPUT
	printf("Hey");

	readStuffs(slaveSocket, input2, keyL);
	readStuffs(slaveSocket, input3, passL);
	
	read(slaveSocket, &next, sizeof(next));
	printf("My hands are typing words lsdf;laksdjf;l.\n");
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/gtgm";
	printStandard(slaveSocket,stringA);
	//database code
	dataGetG(slaveSocket,input2, input3);

	close(slaveSocket);
}


void gtal(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];	//PASS
	char input3[passL];	//NAME


	//GET INPUT
	printf("Hey");

	readStuffs(slaveSocket, input2, keyL);
	readStuffs(slaveSocket, input3, passL);
	
	read(slaveSocket, &next, sizeof(next));
	printf("My hands are typing words lsdf;laksdjf;l.\n");
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/gtal";
	printStandard(slaveSocket,stringA);
	//database code



	dataGetFR(slaveSocket,input2, input3);	
	dataGetG(slaveSocket,input2, input3);
	

	close(slaveSocket);
}

/////////////////////////////////

void chck(int slaveSocket)
{
	
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[5]; //# of events

	//GET INPUT


	readStuffs(slaveSocket, input2, 5);
	
	int temp = atoi(input2);	

	if(temp <= 0)
	{
		char stringA[6] = "/chck";
		printStandard(slaveSocket,stringA);
		write(slaveSocket, "ISVALID0",8);
		close(slaveSocket);
		return;
	}	 

	char input3[temp * evKeyL];
	
	readStuffs(slaveSocket, input3,(temp * evKeyL));
	read(slaveSocket, &next, sizeof(next));

	readRemainder(slaveSocket);


	//OTHER
	char stringA[6] = "/chck";
	printStandard(slaveSocket,stringA);

	//database code
	//dataDeleteUser(slaveSocket,input2,input3);
	dataCheck(slaveSocket, input3);
	close(slaveSocket);

}

void addf(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];	//PASS
	char input3[passL];	//NAME
	char input4[aliasL];	//ALIAS


	//GET INPUT
	printf("Hey");

	readStuffs(slaveSocket, input2, keyL);
	readStuffs(slaveSocket, input3, passL);
	readStuffs(slaveSocket, input4, aliasL);
	
	read(slaveSocket, &next, sizeof(next));
	printf("My hands are typing words lsdf;laksdjf;l.\n");
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/addf";

	printStandard(slaveSocket,stringA);
	//database code
	dataAddFriend(slaveSocket,input2, input3, input4);

	close(slaveSocket);
}

void delf(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];	//PASS
	char input3[passL];	//NAME
	char input4[aliasL];	//ALIAS


	//GET INPUT
	printf("Hey");

	readStuffs(slaveSocket, input2, keyL);
	readStuffs(slaveSocket, input3, passL);
	readStuffs(slaveSocket, input4, aliasL);
	
	read(slaveSocket, &next, sizeof(next));
	printf("My hands are typing words lsdf;laksdjf;l.\n");
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/addf";
	printStandard(slaveSocket,stringA);
	//database code
	dataDeleteFriend(slaveSocket,input2, input3, input4);

	close(slaveSocket);
}

//CREATE USER
void crus(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[passL];	//PASS
	char input3[nameL];	//NAME
	char input4[aliasL];	//ALIAS
	char input5[ageL]; 	//AGE
	char input6[genderL];		//GENDER	
	//char input7[101];	//DESC

	//GET INPUT
	printf("Hey");

	readStuffs(slaveSocket, input2, passL);
	readStuffs(slaveSocket, input3, nameL);
	readStuffs(slaveSocket, input4, aliasL);
	readStuffs(slaveSocket, input5, ageL);
	readStuffs(slaveSocket, input6, genderL);
	
	read(slaveSocket, &next, sizeof(next));
	printf("My hands are typing words lsdf;laksdjf;l.\n");
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/crus";
	printStandard(slaveSocket,stringA);
	//database code
	dataCreateUser(slaveSocket,input2, input3, input4, input5, input6);

	close(slaveSocket);
}



void crup(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL]; //key

	//GET INPUT
	printf("Hey");

	readStuffs(slaveSocket, input2, keyL);
	
	read(slaveSocket, &next, sizeof(next));
	printf("My hands are typing words lsdf;laksdjf;l.\n");
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/crup";
	printStandard(slaveSocket,stringA);
	//database code
	getCreatedNumAttending(slaveSocket,input2);

	close(slaveSocket);
}

void deus(int slaveSocket)
{
	
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];		//KEY
	char input3[passL];	//PASS

	//GET INPUT


	 readStuffs(slaveSocket, input2, keyL);
	 readStuffs(slaveSocket, input3, passL);
	
	read(slaveSocket, &next, sizeof(next));

	readRemainder(slaveSocket);


	//OTHER
	char stringA[6] = "/deus";
	printStandard(slaveSocket,stringA);

	//database code
	dataDeleteUser(slaveSocket,input2,input3);
	close(slaveSocket);

}



//CREATE EVENT
void crev(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];		//KEY
	char input3[passL];	//PASS
	char input4[sportL];	//SPORT
	char input5[locL];	//LOC
	char input6[dateL];	//DATE
	char input7[timeL];		//TIME
	char input8[summL];	//SUMM
	char input9[competeL];		//compete
	char input10[titleL];
	char input11[attendL];
	char input12[dateL];	//end date
	char input13[paramL];


	//GET INPUT

	readStuffs(slaveSocket, input2, keyL);
	readStuffs(slaveSocket, input3, passL);
	readStuffs(slaveSocket, input4, sportL);
	readStuffs(slaveSocket, input5, locL);
	readStuffs(slaveSocket, input6, dateL);
	readStuffs(slaveSocket, input7, timeL);
	readStuffs(slaveSocket, input8, summL);
	readStuffs(slaveSocket, input9, competeL);
	readStuffs(slaveSocket, input10, titleL);
	readStuffs(slaveSocket, input11, attendL);
	readStuffs(slaveSocket, input12, dateL);
	readStuffs(slaveSocket, input13, paramL);
	
	read(slaveSocket, &next, sizeof(next));
	
	printf("HEYOH\n");

	//OTHER
	char stringA[6] = "/crev";
	
	printf("??\n");
	//database code
	
	time_t now;
	struct tm tstruct;
	char date[80], timeA[80];

	now = time(0);
	tstruct = *localtime(&now);
	strftime(date,sizeof(date),"%Y%m%d",&tstruct);
	strftime(timeA,sizeof(timeA), "%H%M", &tstruct);

	struct tm newTM;
	
	printf("yo\n");
	if(atoi(input6) < atoi(date) || (atoi(date) == atoi(input6) && atoi(input7) < atoi(timeA)))
	{
		printf("???\n");
		invl(slaveSocket);
		printf("what?\n");
		return;
	}
	readRemainder(slaveSocket);
	printStandard(slaveSocket,stringA);
	
	int i = 0;
	std::string temp;

	if(strncmp(input13,"0", 1) == 0)
	{
		printf("yo2\n");
		dataCreateEvent(slaveSocket,input2,input3,input4,input5,input6,input7,input8,input9,input10,input11);
		
		temp = "~" + std::string(input6) + "~";		
		write(slaveSocket,temp.c_str(),dateL);
	}
	else {
		while(atoi(input6) < atoi(input12) && i < 31)
		{
		
			dataCreateEvent(slaveSocket,input2,input3,input4,input5,input6,input7,input8,input9,input10,input11);

			temp = "~" + std::string(input6) + "~";		
			write(slaveSocket,temp.c_str(),dateL);
			

			i++;
		
			memset(&newTM, 0, sizeof(struct tm));
			strptime(input6, "%Y%m%d", &newTM);	
		
			if(strncmp(input13, "1",1)) // weekly add 1 week
			{
				newTM.tm_mday += 7;
				mktime(&newTM);
				strftime(input6,sizeof(input6),"%Y%m%d", &newTM);
			}
			else 	//daily, add one day
			{
				newTM.tm_mday += 1;
				mktime(&newTM);
				strftime(input6,sizeof(input6),"%Y%m%d", &newTM);
			}
		}
	}

	close(slaveSocket);
}


//UPDATE USER
void upus(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];		//KEY
	char input3[passL];		//PASS
	char input4[nameL];		//NAME
	char input5[aliasL];	//ALIAS
	char input6[ageL];		//AGE
	char input7[genderL];		//GENDER
	char input8[descL];		//DESC
	char input9[passL];		//new pass
	char input10[genderL];  // new pref

	
	//GET INPUT

	 readStuffs(slaveSocket, input2, keyL);
	 readStuffs(slaveSocket, input3, passL);
	 readStuffs(slaveSocket, input4, nameL);
	 readStuffs(slaveSocket, input5, aliasL);
	 readStuffs(slaveSocket, input6, ageL);
	 readStuffs(slaveSocket, input7, genderL);
	 readStuffs(slaveSocket, input8, descL);
	 readStuffs(slaveSocket, input9, passL);
	 readStuffs(slaveSocket, input10, genderL);
	
	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);
	//OTHER
	char stringA[6] = "/upus";
	printStandard(slaveSocket,stringA);
	//database code
	dataUpdateUser(slaveSocket,input2,input3,input4,input5,input6,input7,input8,input9, input10);

	close(slaveSocket);
}


//UPDATE EVENT
void upev(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];		//KEY
	char input3[passL];	//PASS
	char input4[evKeyL];		//EVKEY
	char input5[sportL];	//SPORT
	char input6[locL];	//LOC
	char input7[dateL];	//DATE
	char input8[timeL];		//TIME
	char input9[summL];	//SUMM
	char input10[competeL];	//SKILL
	char input11[titleL];
	char input12[attendL];


	//GET INPUT

	 readStuffs(slaveSocket, input2, keyL);
	 readStuffs(slaveSocket, input3, passL);
	 readStuffs(slaveSocket, input4, evKeyL);
	 readStuffs(slaveSocket, input5, sportL);
	 readStuffs(slaveSocket, input6, locL);
	 readStuffs(slaveSocket, input7, dateL);
	 readStuffs(slaveSocket, input8, timeL);
	 readStuffs(slaveSocket, input9, summL);
	 readStuffs(slaveSocket, input10, competeL);
	 readStuffs(slaveSocket, input11, titleL);
	 readStuffs(slaveSocket, input12, attendL);

	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);
	//OTHER
	char stringA[6] = "/upev";
	printStandard(slaveSocket,stringA);
	//database code
	dataUpdateEvent(slaveSocket,input2,input3,input4,input5,input6,input7,input8,input9,input10,input11, input12);

	close(slaveSocket);
}


//DELETE EVENT
void deev(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];		//KEY
	char input3[passL];	//PASS
	char input4[evKeyL];		//EVKEY


	//GET INPUT

	 readStuffs(slaveSocket, input2, keyL);
	 readStuffs(slaveSocket, input3, passL);
	 readStuffs(slaveSocket, input4, evKeyL);

	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/deev";
	printStandard(slaveSocket,stringA);
	//database code
	dataDeleteEvent(slaveSocket,input2,input3,input4);

	close(slaveSocket);
}


//GET EVENT
void gtev(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[sportL];	//SPORT
	char input3[locL];	//LOC
	char input4[dateL];	//DATE
	char input5[timeL];		//TIME
	char input6[competeL];		//competeL
	char input7[titleL];
	char input8[dateL];
	char input9[timeL];


	//GET INPUT

	 readStuffs(slaveSocket, input2, sportL);
	 readStuffs(slaveSocket, input3, locL);
	 readStuffs(slaveSocket, input4, dateL);
	 readStuffs(slaveSocket, input5, timeL);
	 readStuffs(slaveSocket, input6, competeL);
	 readStuffs(slaveSocket, input7, titleL);
	 readStuffs(slaveSocket, input8, dateL);
	 readStuffs(slaveSocket, input9, timeL);

	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/gtev";
	printStandard(slaveSocket,stringA);
	//database code
	dataGetEvent(slaveSocket,input2,input3,input4,input5,input6,input7,input8, input9);

	close(slaveSocket);
}


void join(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];		//KEY
	char input3[passL];	//PASS
	char input4[evKeyL];	//eventkey

	//GET INPUT

	 readStuffs(slaveSocket, input2, keyL);
	 readStuffs(slaveSocket, input3, passL);
	 readStuffs(slaveSocket, input4, evKeyL);

	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/join";
	printStandard(slaveSocket,stringA);
	//database code
	//dataDeleteEvent(slaveSocket,input2,input3,input4);
	dataJoinEvent(slaveSocket,input2,input3,input4, 0);
	
	close(slaveSocket);
}


void unjn(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];		//KEY
	char input3[passL];	//PASS
	char input4[evKeyL];	//eventkey

	//GET INPUT

	 readStuffs(slaveSocket, input2, keyL);
	 readStuffs(slaveSocket, input3, passL);
	 readStuffs(slaveSocket, input4, evKeyL);

	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/unjn";
	printStandard(slaveSocket,stringA);
	//database code
	//dataDeleteEvent(slaveSocket,input2,input3,input4);
	dataUnJoinEvent(slaveSocket,input2,input3,input4);
	
	close(slaveSocket);
}

void lgon(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[aliasL];		//alias
	char input3[passL];		//PASS


	//GET INPUT

	 readStuffs(slaveSocket, input2, aliasL);
	 readStuffs(slaveSocket, input3, passL);

	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/lgon";
	printStandard(slaveSocket,stringA);
	//database code
	dataLogOn(slaveSocket,input2,input3);

	close(slaveSocket);
}

void upts(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];
	char input3[passL];
	char input4[sportL];
	char input5[sportL];
	char input6[sportL];


	//GET INPUT
	readStuffs(slaveSocket, input2, keyL);
	readStuffs(slaveSocket, input3, passL);
	readStuffs(slaveSocket, input4, sportL);
	readStuffs(slaveSocket, input5, sportL);
	readStuffs(slaveSocket, input6, sportL);

	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/upts";
	printStandard(slaveSocket, stringA);
	//database code
	updateTopSports(slaveSocket, input2, input3, input4, input5, input6);

	close(slaveSocket);
}

//INVALID INPUT
void invl(int slaveSocket)
{
	readRemainder(slaveSocket);
	printf("INVALID INPUT\n");
	char stringA[6] = "/invl";
	printStandard(slaveSocket,stringA);
	write(slaveSocket,"INVALID",7);
	close(slaveSocket);
}

void printStandard(int slaveSocket,char * input1)
{
	printf("?\n");
	write(slaveSocket,"HTTP/1.1",8);
	write(slaveSocket," ",1);
	write(slaveSocket,"200",3);
	write(slaveSocket," ",1);
	write(slaveSocket,input1,strlen(input1));
	write(slaveSocket," ",1);
	write(slaveSocket,"follows",7);
	write(slaveSocket,"\r\n",2);
	write(slaveSocket,"Server:",7);
	write(slaveSocket," ",1);
	write(slaveSocket,"CS",2);
	write(slaveSocket," ",1);
	write(slaveSocket,"307",3);
	write(slaveSocket," ",1);
	write(slaveSocket,"GRPA",4);
	write(slaveSocket,"\r\n",2);
	write(slaveSocket,"\r\n",2);
}

void readRemainder(int slaveSocket)
{
		unsigned char next, last;
		last = '/';
		//bool end = false;
		while(1)
		{

			read(slaveSocket, &next, sizeof(next));
			printf("%c", next);

			if (next == '\n' && last == '\r')
			{
				read(slaveSocket, &last, sizeof(last));
				read(slaveSocket, &next, sizeof(next));
				if(next == '\n' && last == '\r')
				{
					printf("\n");
					break;
				}
			}
			last = next;
		}
}

void readStuffs(int slaveSocket, char * input, int length)
{
	char next;
	int counter;
	
	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < length-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input[counter] = next;
	}	
	input[counter] = '\0';
	printf("%s\n",input);
}
