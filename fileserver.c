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
#define LENGTH 512 

void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main ()
{
	/* Defining Variables */

	long unsigned int totB;
	int sockfd; 
	int nsockfd; 
	int num;
	int sin_size; 
	struct sockaddr_in addr_local; /* client addr */
	struct sockaddr_in addr_remote; /* server addr */
	char revbuf[LENGTH]; // Receiver buffer
	//int port=0;



	/* Get the Socket file descriptor */
	if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) == -1 )
	{
		fprintf(stderr, "ERROR: Failed to obtain Socket Descriptor. (errno = %d)\n", errno);
		exit(1);
	}
	else 
		printf("[Server] Obtaining socket descriptor successfully.\n");




	/* Fill the client socket address struct */
	addr_local.sin_family = AF_INET; // Protocol Family
	addr_local.sin_port = htons(PORT); // Port number
	addr_local.sin_addr.s_addr = INADDR_ANY; // AutoFill local address
	bzero(&(addr_local.sin_zero), 8); // Flush the rest of struct

	/* Bind a special Port */
	if( bind(sockfd, (struct sockaddr*)&addr_local, sizeof(struct sockaddr)) == -1 )
	{
		fprintf(stderr, "ERROR: Failed to bind Port. (errno = %d)\n", errno);
		exit(1);
	}
	 
	printf("[Server] Binded port %d sucessfully.\n",PORT);

	/* Listen remote connect/calling */
	if(listen(sockfd,BACKLOG) == -1)
	{
		fprintf(stderr, "ERROR: Failed to listen Port. (errno = %d)\n", errno);
		exit(1);
	}
	
	printf ("[Server] Listening the port %d successfully.\n", PORT);

	while(1)
	{
		sin_size = sizeof(struct sockaddr_in);

		/* Wait a connection, and obtain a new socket file despriptor for single connection */
		if ((nsockfd = accept(sockfd, (struct sockaddr *)&addr_remote, &sin_size)) == -1) 
		{
		    fprintf(stderr, "ERROR: Obtaining new Socket Despcritor. (errno = %d)\n", errno);
			exit(1);
		}
		
		printf("[Server] Server has got connected from %s.\n", inet_ntoa(addr_remote.sin_addr));

		/*Receive File from Client */
		char* fr_name = "/home/aryan/Desktop/receive.txt";
		FILE *fr = fopen(fr_name, "a");
		if(fr == NULL)
			printf("File %s Cannot be opened file on server.\n", fr_name);
		else
		{

			/* Call the Script */
			//system("cd ; chmod +x script.sh ; ./script.sh");
			


			time_t curtime, curtime2;
			/* Get the current time. */
			curtime = time (NULL);




			bzero(revbuf, LENGTH); 
			int nRead = 0;
			while((nRead = recv(nsockfd, revbuf, LENGTH, 0)) > 0) 
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
			fclose(fr); 
		}


	}
}
