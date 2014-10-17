//***************handler

else if(strcmp(input1,"/deus\0" == 0))
{
	deus(slaveSocket);
}





void deus(int);

void deus(int slaveSocket)
{
	
	unsigned char next;
	int counter;
	//INPUT AREAS
	char input2[5];		//KEY
	char input3[21];	//PASS

	//GET INPUT


	read(slaveSocket, &next, sizeof(next));

	for(counter = 0; counter < 4; counter++)
	{
		read(slaveSocket, &next, sizeof(next));
		input2[counter] = next;
	}	

	input2[counter] = '\0';
	read(slaveSocket, &next, sizeof(next));


	for(counter = 0; counter < 20; counter++)
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



//**************************tbinclude.h

void dataDeleteUser(int, char *, char *);

//*************************databaseInterface.c

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

