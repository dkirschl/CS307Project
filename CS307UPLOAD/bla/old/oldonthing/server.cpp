//Dylan Smith's Code for CS307 Project

void child(int sig);
void createThreadForEachRequest(int masterSocket);
void dispatchHTTP(int slaveSocket);
void* dispatchHTTP(void* slaveSocket);
void work( int slaveSocket);


int defaultPORT = 39001;
int QueueLength = 20;

const char * usage =
"                                                               \n"
"	projectServer useage:					\n"
"	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
"								\n"
"	Type in help to read this message.			\n"
"								\n"
"	This program will respond to queries made by an app 	\n"
"	written for android phones. It will send them a text 	\n"
"	file with information on class times and student group	\n"
"	meetings.						\n"
"								\n"
"	~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n"
"								\n"
"								\n";


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


#include "handler.h"


int
main( int argc, char ** argv )
{

 	if ( argc == 2 && strcmp(argv[1],"help\0") == 0) {
 		fprintf( stderr, "%s", usage );
 	   	exit( -1 );
  	}
	
	struct sigaction signalAction;
	signalAction.sa_handler = child;

	
  	// Set the IP address and port for this server
 	struct sockaddr_in serverIPAddress; 
 	memset( &serverIPAddress, 0, sizeof(serverIPAddress) );
  	serverIPAddress.sin_family = AF_INET;
  	serverIPAddress.sin_addr.s_addr = INADDR_ANY;
  	serverIPAddress.sin_port = htons((u_short) defaultPORT);
  
	
  	// Allocate a socket
  	int masterSocket =  socket(PF_INET, SOCK_STREAM, 0);
  	if( masterSocket < 0) 
	{
  		perror("socket");
    		exit( -1 );
  	}

	
	// Set socket options to reuse port. Otherwise we will
  	// have to wait about 2 minutes before reusing the sae port number
  	int optval = 1; 
  	int err = setsockopt(masterSocket, SOL_SOCKET, SO_REUSEADDR,(char *) &optval, sizeof( int ) );
   
	
  	// Bind the socket to the IP address and port
  	int error = bind( masterSocket,(struct sockaddr *)&serverIPAddress,sizeof(serverIPAddress) );
  	if( error )
	{
   		perror("bind");
    		exit( -1 );
  	}
  
  	// Put socket in listening mode and set the 
  	// size of the queue of unproce ssed connections
  	error = listen( masterSocket, QueueLength);
  	if( error ) 
	{
    		perror("listen");
    		exit( -1 );
  	}
	

	//Enter into infinate loop, wait till death.
	createThreadForEachRequest( masterSocket );
}

void child(int sig)
{
	while(waitpid(-1, NULL, WNOHANG) > 0); 
}

void createThreadForEachRequest ( int masterSocket)
{
	printf("Here\n");
	while(1)
	{
		printf("making\n");
		struct sockaddr_in clientIPAddress;
    		int alen = sizeof( clientIPAddress );
    		int slaveSocket = accept( masterSocket,(struct sockaddr*) &clientIPAddress,(socklen_t*) &alen);
		printf("making attempt\n");
		pthread_t thread;
		if(slaveSocket >= 0)
		{
			printf("making complete\n");
			pthread_attr_t attr;
			pthread_attr_init(&attr);
			pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
			pthread_create(&thread, &attr, dispatchHTTP, (void*) &slaveSocket);
		}
		else
		{
			close(slaveSocket);
		}
	}
}

void* dispatchHTTP(void* slaveSocket)
{
	dispatchHTTP( *(int *)slaveSocket);
	return NULL;
}


void dispatchHTTP(int slaveSocket)
{
	printf("working\n");
	work(slaveSocket);
	shutdown(slaveSocket,2);
	close(slaveSocket);
}

