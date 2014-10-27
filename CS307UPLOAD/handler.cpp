//Message handler By Dylan Smith
/*
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <time.h>
#include <pthread.h>
#include <signal.h>
#include <sys/wait.h>
#include <errno.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "databaseInterface.h"
*/

#include "tbinclude.h"

void crus(int);
void deus(int);
void crev(int);
void upus(int);
void upev(int);
void deev(int);
void gtev(int);
void invl(int);
void join(int);
void unjn(int); 

void printStandard(int,char *);
void readRemainder(int);

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
	/*
	const int maxChars = 1000;
	char input1[100],input2[100],input3[100],input4[100],input5[100],input6[100];
	int length = 0;
	unsigned char next;
	unsigned char last = 0;
	int n;
	int tempi;
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
	else if(strcmp(input1,"/join") == 0)
	{
		join(slaveSocket);
	}
	else if(strcmp(input1,"/unjn") == 0) //NEW>>>>>>>>
	{
		unjn(slaveSocket);		//NEW>>>>>>
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
	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < passL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input2[counter] = next;
	}	
	input2[counter] = '\0';
	printf("%s\n",input2);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < nameL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input3[counter] = next;
	}	
	input3[counter] = '\0';
	printf("%s\n",input3);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < aliasL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input4[counter] = next;
	}	
	input4[counter] = '\0';
	printf("%s\n",input4);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < ageL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input5[counter] = next;
	}	
	input5[counter] = '\0';
	printf("%s\n",input5);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < genderL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input6[counter] = next;
	}
	input6[counter] = '\0';	
	printf("%s\n",input6);

	read(slaveSocket, &next, sizeof(next));

	/*for(counter = 0; counter < 100; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input7[counter] = next;
	}
	*/	
	//input7[counter] = '\0';
	//printf("%s\n",input7);	
	printf("My hands are typing words lsdf;laksdjf;l.\n");
	//read(slaveSocket, &next, sizeof(next));
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


	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < keyL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input2[counter] = next;
	}	

	input2[counter] = '\0';
	read(slaveSocket, &next, sizeof(next));


	for(counter = 0; counter < passL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input3[counter] = next;
	}	

	input3[counter] = '\0';
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


	//GET INPUT

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < keyL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input2[counter] = next;
	}	
	input2[counter] = '\0';
	printf("%s\n",input2);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < passL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input3[counter] = next;
	}	
	input3[counter] = '\0';
	printf("%s\n",input3);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < sportL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input4[counter] = next;
	}	
	input4[counter] = '\0';
	printf("%s\n",input4);
	
	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < locL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input5[counter] = next;
	}	
	input5[counter] = '\0';
	printf("%s\n",input5);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < dateL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input6[counter] = next;
	}	
	input6[counter] = '\0';
	printf("%s\n",input6);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < timeL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input7[counter] = next;
	}	
	input7[counter] = '\0';
	printf("%s\n",input7);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < summL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input8[counter] = next;
	}	
	input8[counter] = '\0';
	printf("%s\n",input8);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < competeL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input9[counter] = next;
	}	
	input9[counter] = '\0';
	printf("%s\n",input9);
	
	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);
	printf("HEYOH\n");

	//OTHER
	char stringA[6] = "/crev";
	printStandard(slaveSocket,stringA);
	//database code
	dataCreateEvent(slaveSocket,input2,input3,input4,input5,input6,input7,input8,input9);

	close(slaveSocket);
}


//UPDATE USER
void upus(int slaveSocket)
{
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[keyL];		//KEY
	char input3[passL];	//PASS
	char input4[nameL];	//NAME
	char input5[aliasL];	//ALIAS
	char input6[ageL];		//AGE
	char input7[genderL];		//GENDER
	char input8[descL];	//DESC

	
	//GET INPUT

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < keyL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input2[counter] = next;
	}	
	input2[counter] = '\0';
	printf("%s\n",input2);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < passL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input3[counter] = next;
	}	
	input3[counter] = '\0';
	printf("%s\n",input3);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < nameL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input4[counter] = next;
	}	
	input4[counter] = '\0';
	printf("%s\n",input4);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < aliasL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input5[counter] = next;
	}	
	input5[counter] = '\0';
	printf("%s\n",input5);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < ageL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input6[counter] = next;
	}	
	input6[counter] = '\0';
	printf("%s\n",input6);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < genderL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input7[counter] = next;
	}	
	input7[counter] = '\0';
	printf("%s\n",input7);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < descL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input8[counter] = next;
	}	
	input8[counter] = '\0';
	printf("%s\n",input8);

	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);
	//OTHER
	char stringA[6] = "/upus";
	printStandard(slaveSocket,stringA);
	//database code
	dataUpdateUser(slaveSocket,input2,input3,input4,input5,input6,input7,input8);

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


	//GET INPUT

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < keyL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input2[counter] = next;
	}	
	input2[counter] = '\0';
	printf("%s\n",input2);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < passL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input3[counter] = next;
	}	
	input3[counter] = '\0';
	printf("%s\n",input3);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < evKey-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input4[counter] = next;
	}	
	input4[counter] = '\0';
	printf("%s\n",input4);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < sportL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input5[counter] = next;
	}	
	input5[counter] = '\0';
	printf("%s\n",input5);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < locL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input6[counter] = next;
	}	
	input6[counter] = '\0';
	printf("%s\n",input6);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < dateL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input7[counter] = next;
	}	
	input7[counter] = '\0';
	printf("%s\n",input7);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < timeL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input8[counter] = next;
	}	
	input8[counter] = '\0';
	printf("%s\n",input8);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < summL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input9[counter] = next;
	}	
	input9[counter] = '\0';
	printf("%s\n",input9);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < competeL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input10[counter] = next;
	}	
	input10[counter] = '\0';
	printf("%s\n",input10);

	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);
	//OTHER
	char stringA[6] = "/upev";
	printStandard(slaveSocket,stringA);
	//database code
	dataUpdateEvent(slaveSocket,input2,input3,input4,input5,input6,input7,input8,input9,input10);

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

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < keyL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input2[counter] = next;
	}	
	input2[counter] = '\0';
	printf("%s\n",input2);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < passL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input3[counter] = next;
	}	
	input3[counter] = '\0';
	printf("%s\n",input3);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < evKeyL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input4[counter] = next;
	}	
	input4[counter] = '\0';
	printf("%s\n",input4);	

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


	//GET INPUT

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < sportL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input2[counter] = next;
	}	
	input2[counter] = '\0';
	printf("%s\n",input2);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < locL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input3[counter] = next;
	}	
	input3[counter] = '\0';
	printf("%s\n",input3);
	
	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < dateL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input4[counter] = next;
	}	
	input4[counter] = '\0';
	printf("%s\n",input4);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < timeL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input5[counter] = next;
	}	
	input5[counter] = '\0';
	printf("%s\n",input5);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < competeL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input6[counter] = next;
	}	
	input6[counter] = '\0';
	printf("%s\n",input6);

	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/gtev";
	printStandard(slaveSocket,stringA);
	//database code
	dataGetEvent(slaveSocket,input2,input3,input4,input5,input6);

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

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < keyL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input2[counter] = next;
	}	
	input2[counter] = '\0';
	printf("%s\n",input2);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < passL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input3[counter] = next;
	}	
	input3[counter] = '\0';
	printf("%s\n",input3);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < evKeyL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input4[counter] = next;
	}	
	input4[counter] = '\0';
	printf("%s\n",input4);

	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/join";
	printStandard(slaveSocket,stringA);
	//database code
	//dataDeleteEvent(slaveSocket,input2,input3,input4);
	dataJoinEvent(slaveSocket,input2,input3);
	
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

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < keyL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input2[counter] = next;
	}	
	input2[counter] = '\0';
	printf("%s\n",input2);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < passL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input3[counter] = next;
	}	
	input3[counter] = '\0';
	printf("%s\n",input3);

	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < evKeyL-1; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input4[counter] = next;
	}	
	input4[counter] = '\0';
	printf("%s\n",input4);

	read(slaveSocket, &next, sizeof(next));
	readRemainder(slaveSocket);

	//OTHER
	char stringA[6] = "/unjn";
	printStandard(slaveSocket,stringA);
	//database code
	//dataDeleteEvent(slaveSocket,input2,input3,input4);
	dataUnJoinEvent(slaveSocket,input2,input3);
	
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

/*
				printf("working attempt\n");

				for(tempi = 0; tempi < 4;tempi++){
					n = read(slaveSocket, &next, sizeof(next));
				}
	
				printf("attempt exists\n");

	for(tempi = 0; tempi < 4; tempi++)
	{
		n = read(slaveSocket, &next, sizeof(char));
		input1[tempi] = next;
	}
	input1[tempi] = '\0';
printf("%s\n", input1);
printf("Attempt passed\n");

	read(slaveSocket, &next, sizeof(next));
	
	if(strcmp(input1,"/add\0") == 0)								//see
	{
		tempi = 0;
		next = 0;
		while(next != '-')
		{
			n = read(slaveSocket, &next, sizeof(next));
			input2[tempi] = next;
			tempi++;
		}
		input2[tempi-1] = '\0';	
printf("%s\n",input2);
		tempi = 0;
		next = 0;
		while(next != '-')
		{	
			n = read(slaveSocket, &next, sizeof(next));
			input3[tempi] = next;
			tempi++;
		}
		input3[tempi-1] = '\0';	
printf("%s\n",input3);
		tempi = 0;
		next = 0;
		while(next != '-')
		{
			n = read(slaveSocket, &next, sizeof(next));
			input4[tempi] = next;
			tempi++;
		}
		input4[tempi-1] = '\0';	
printf("%s\n",input4);
		tempi = 0;
		next = 0;
		while(next != '-')
		{
			n = read(slaveSocket, &next, sizeof(next));
			input5[tempi] = next;
			tempi++;
		}
		input5[tempi-1] = '\0';	
printf("%s\n",input5);
		tempi = 0;
		next = 0;
		while(next != '-')
		{
			n = read(slaveSocket, &next, sizeof(next));
			input6[tempi] = next;
			tempi++;
		}
		input6[tempi-1] = '\0';
printf("%s\n",input6);
		bool end = false;

		while(1)
		{
			//printf("yay\n");
			read(slaveSocket, &next, sizeof(next));
			if(end == true && next == '\n' && last == '\r')
			{
				break;
			}
			else if (next == '\n' && last == '\r')
			{
printf("<crlf>\n");
				break;
			}
			else
			{
				end = false;
			}
			last = next;
		}	
	
	}
	else 												//add
	{
		tempi = 0;
		next = 0;
		while(next != '-')
		{
			n = read(slaveSocket, &next, sizeof(next));
			input2[tempi] = next;
			tempi++;
		}
		input2[tempi-1] = '\0';	
printf("%s\n",input2);
		tempi = 0;
		next = 0;
		while(next != '-')
		{
			n = read(slaveSocket, &next, sizeof(next));
			input3[tempi] = next;
			tempi++;
		}
		input3[tempi-1] = '\0';	
printf("%s\n",input3);
		bool end = false;
		while(1)
		{

			read(slaveSocket, &next, sizeof(next));
			if(end == true && next == '\n' && last == '\r')
			{
				break;
			}
			else if (next == '\n' && last == '\r')
			{
				break;
				end = true;
			}
			else
			{
				end = false;
			}
			last = next;
		}
	}

printf("Hello\n");	

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

	/*
	envvars list:		/add-DAY-TIME-NAME-CLASS-ROOM-
		serverTimeVar	input3
		serverDayVar	input2
		serverNameVar	input4
		serverClassVar	input5
		serverLabRoom	input6

	*/
/*
	int ret;
	setenv("serverDayVar",input2,1);
	setenv("serverTimeVar",input3,1);
	int std;
	std = dup(1);
	dup2(slaveSocket,1);
	char cwd[256];
	char * cwdP;
	cwdP = cwd;
	strcpy(getcwd(cwdP,254),cwd);
	
	if(strcmp(input1,"/add") == 0)
	{
		//set envvars and then execute
		setenv("serverNameVar",input4,1);
		setenv("serverClassVar",input5,1);
		setenv("serverLabRoom",input6,1);
		
		ret = fork();			//fork
		if(ret == -1)			
		{}
		else if(ret == 0){		//if ret == 0
			close(std);	
			strcat(cwdP, "/add.sh\0");	
			execvp(cwdP,NULL);	//	execvp
			exit(-1);
		}
		else
		{				//else parent
			wait(NULL);		//	wait(NULL)
		}
		
	}
	strcpy(getcwd(cwdP,254),cwd);
	
	ret = fork();
	if(ret == -1)
	{}
	else if(ret == 0)
	{
		strcat(cwdP, "/see.sh\0");
		close(std);
		execvp("./see",NULL);
		exit(-1);
	}
	else
	{
		wait(NULL);
	}
	dup2(std,1);
	close(std);
	printf("all complete\n");
	*/
