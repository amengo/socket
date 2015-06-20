#include <stdlib.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <sys/wait.h>
#include <sys/socket.h>
#include <signal.h>
#include <ctype.h>          
#include <arpa/inet.h>
#include <netdb.h>

#define PORT 60000 
#define BACKLOG 5
#define LENGTH 4096

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main (int argc, char **argv)
{
	/* Defining Variables */
	int totb=0;
	int diffs=0;
	float mbs=0.0;
	long unsigned int totB=0;
	char revbuf[LENGTH]; // Receiver buffer
	int nsockfd;
	int  listen_sd, conn_sd;
	int port, len, num, nread;
//	char buff[DIM_BUFF];
	const int on = 1;
	struct sockaddr_in cliaddr, servaddr;
	struct hostent *host;

	/* CONTROLLO ARGOMENTI */

	if(argc!=2)
	{
		printf("Usage: %s port\n", argv[0]);
		exit(1);
	}
	else
	{
		num=0;
		while( argv[1][num]!= '\0' )
		{
			if( (argv[1][num] < '0') || (argv[1][num] > '9') )
			{
				printf("Secondo argomento non intero\n");
				printf("Usage: %s port\n", argv[0]);
				exit(2);
			}
			num++;
		}  	
		port = atoi(argv[1]);
		if (port < 1024 || port > 65535) 
		{
			printf("Error: %s port\n", argv[0]);
			printf("1024 <= port <= 65535\n");
			printf("Usage: %s port\n", argv[0]);
			exit(2);  	
		}

	}

	/* INIZIALIZZAZIONE INDIRIZZO SERVER */

	memset ((char *)&servaddr, 0, sizeof(servaddr));
	servaddr.sin_family = AF_INET;
	servaddr.sin_addr.s_addr = INADDR_ANY;
	servaddr.sin_port = htons(port);


	/* CREAZIONE E SETTAGGI SOCKET D'ASCOLTO */

	listen_sd=socket(AF_INET, SOCK_STREAM, 0);
	if(listen_sd <0)
	{
		perror("creazione socket "); 
		exit(1);
	}
	printf("Server: creata la socket d'ascolto per le richieste di ordinamento, fd=%d\n", listen_sd);

	if(setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, &on, sizeof(on)) < 0)
	{
		perror("set opzioni socket d'ascolto"); 
		exit(1);
	}
	printf("Server: set opzioni socket d'ascolto ok\n");

	if(bind(listen_sd,(struct sockaddr *) &servaddr, sizeof(servaddr)) < 0)
	{
		perror("bind socket d'ascolto");
		exit(1);
	}
	printf("Server: bind socket d'ascolto ok\n");


	if (listen(listen_sd, 5)<0)
	{
		perror("listen"); 
		exit(1);
	}
	printf("Server: listen ok\n");





	while(1)
	{
		printf("Server: in attesa di richieste:\n");
		len=sizeof(cliaddr);

		if((conn_sd=accept(listen_sd,(struct sockaddr *)&cliaddr,&len))<0)
		{
			if (errno==EINTR) //problema non della socket
			{
				perror("Forzo la continuazione della accept");
				continue;
			}
			else
			{
				exit(1);		
			}		
		}

		printf("[Server] Server has got connected.\n");

		/*Receive File from Client */
#ifdef FILEW 
		char* fr_name = "/home/aryan/Desktop/receive.txt";
		FILE *fr = fopen(fr_name, "a");
		if(fr == NULL)
			printf("File %s Cannot be opened file on server.\n", fr_name);
		else
		{

#endif
			/* Call the Script */
			//system("cd ; chmod +x script.sh ; ./script.sh");
			


			time_t curtime, curtime2;
			/* Get the current time. */
			curtime = time (NULL);




			bzero(revbuf, LENGTH); 
			int nRead = 0;
			while((nRead = recv(conn_sd, revbuf, LENGTH, 0)) > 0) 
			{
			    /*int write_sz = fwrite(revbuf, sizeof(char), nRead, fr);
				if(write_sz < nRead)
			    {
			        error("File write failed on server.\n");
			    }*/
				bzero(revbuf, LENGTH);
				totB= totB+nRead;
				/*if (nRead == 0 || nRead != 512) 
				{
					break;
				}*/
			}
			if(nRead < 0)
		    {
		        if (errno == EAGAIN)
	        	{
	                printf("recv() timed out.\n");
	            }
	            else
	            {
	                fprintf(stderr, "recv() failed due to errno = %d\n", errno);
					exit(1);
	            }
        	}
			printf("Ok received from client!\n");
			curtime2=time(NULL);
			printf("tempo 1: %ld   tempo 2:  %ld!\n", curtime, curtime2);
			printf("totB: %lu\n", totB);
			totb=totB*8;
			diffs=curtime2-curtime;
			mbs=(float)totb/(float)(diffs);
			mbs=mbs/1000;
			mbs=mbs/1000;
			printf("sec: %d\n",diffs);
			printf("Mbit/sec: %f\n",mbs); 
			totB=0;			
#ifdef FILEW 
			fclose(fr); 
		}
#endif

	}
}
