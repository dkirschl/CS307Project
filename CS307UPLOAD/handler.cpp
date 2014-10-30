//Message handler By Dylan Smith
#include "tbinclude.h"

void crus(int);
void deus(int);
void crev(int);//
void upus(int);
void upev(int);//
void deev(int);//
void gtev(int);//
void invl(int);
void join(int);
void unjn(int); 
void lgon(int);

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
	else 										//INVALID INPUT
	{
		invl(slaveSocket);
	}
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
	
	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);
	printf("HEYOH\n");

	//OTHER
	char stringA[6] = "/crev";
	printStandard(slaveSocket,stringA);
	//database code
	dataCreateEvent(slaveSocket,input2,input3,input4,input5,input6,input7,input8,input9,input10);

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

	
	//GET INPUT

	 readStuffs(slaveSocket, input2, keyL);
	 readStuffs(slaveSocket, input3, passL);
	 readStuffs(slaveSocket, input4, nameL);
	 readStuffs(slaveSocket, input5, aliasL);
	 readStuffs(slaveSocket, input6, ageL);
	 readStuffs(slaveSocket, input7, genderL);
	 readStuffs(slaveSocket, input8, descL);
	 readStuffs(slaveSocket, input9, passL);
	
	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);
	//OTHER
	char stringA[6] = "/upus";
	printStandard(slaveSocket,stringA);
	//database code
	dataUpdateUser(slaveSocket,input2,input3,input4,input5,input6,input7,input8,input9);

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

	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);
	//OTHER
	char stringA[6] = "/upev";
	printStandard(slaveSocket,stringA);
	//database code
	dataUpdateEvent(slaveSocket,input2,input3,input4,input5,input6,input7,input8,input9,input10,input11);

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


	//GET INPUT

	 readStuffs(slaveSocket, input2, sportL);
	 readStuffs(slaveSocket, input3, locL);
	 readStuffs(slaveSocket, input4, dateL);
	 readStuffs(slaveSocket, input5, timeL);
	 readStuffs(slaveSocket, input6, competeL);
	 readStuffs(slaveSocket, input7, titleL);

	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/gtev";
	printStandard(slaveSocket,stringA);
	//database code
	dataGetEvent(slaveSocket,input2,input3,input4,input5,input6,input7);

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
	dataJoinEvent(slaveSocket,input2,input3,input4);
	
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


//INVALID INPUT
void invl(int slaveSocket)
{
	readRemainder(slaveSocket);
	printf("INVALID INPUT");
	char stringA[6] = "/invl";
	printStandard(slaveSocket,stringA);
	write(slaveSocket,"INVALID",7);
	close(slaveSocket);
}

void printStandard(int slaveSocket,char * input1)
{
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
			printf("%c\n", next);

			if (next == '\n' && last == '\r')
			{
				read(slaveSocket, &last, sizeof(last));
				read(slaveSocket, &next, sizeof(next));
				if(next == '\n' && last == '\r')
				{
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
