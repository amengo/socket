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

#define PORT 61000
#define LENGTH 1024 


void error(const char *msg)
{
	perror(msg);
	exit(1);
}

int main(int argc, char *argv[])
{
	/* Variable Definition */
	int sd, nread; 
	int nsd;
	char revbuf[LENGTH]; 
	struct hostent *host;
	struct sockaddr_in servaddr;


	/* CONTROLLO ARGOMENTI */

	if(argc!=3)
	{
		printf("Usage:%s serverAddress serverPort\n", argv[0]);
		exit(1);
	}

	/* INIZIALIZZAZIONE INDIRIZZO SERVER */

	memset((char *)&servaddr, 0, sizeof(struct sockaddr_in));
	servaddr.sin_family = AF_INET;
	host = gethostbyname(argv[1]);
	if (host == NULL)
	{
		printf("%s not found in /etc/hosts and in DNS servers\n", argv[1]);
		exit(1);
	}

	nread=0;
	while( argv[2][nread]!= '\0' )
	{
		if( (argv[2][nread] < '0') || (argv[2][nread] > '9') )
		{
			printf("Secondo argomento non intero\n");
			printf("Usage:%s serverAddress serverPort\n", argv[0]);
			exit(2);
		}
		nread++;
	}  	

	servaddr.sin_addr.s_addr=((struct in_addr*) (host->h_addr))->s_addr;
	servaddr.sin_port = htons(atoi(argv[2]));

	if (servaddr.sin_port < 1024 || servaddr.sin_port > 65535)
	{
		printf("Error: %s port\n", argv[2]);
		printf("1024 <= port <= 65535\n");
		printf("Usage:%s serverAddress serverPort\n", argv[0]);
		exit(2);  	
	}

	/* CREAZIONE SOCKET */

	sd=socket(AF_INET, SOCK_STREAM, 0);
	if(sd<0) 
	{
		perror("apertura socket"); 
		exit(1);
	}
	printf("Client: creata la socket sd=%d\n", sd);

	//operazione di bind implicita nella connect
	if(connect(sd,(struct sockaddr *) &servaddr, sizeof(struct sockaddr)) < 0)
	{ 
		perror("connect"); 
		exit(1);
	}
	printf("Client: connect ok\n");





	/* Send File to Server */
	char* fs_name = "/home/ubuntu/Desktop/socket/fileserver.c";
	char sdbuf[LENGTH]; 
	printf("[Client] Sending %s to the Server... ", fs_name);
	FILE *fs = fopen(fs_name, "r");
	if(fs == NULL)
	{
		printf("ERROR: File %s not found.\n", fs_name);
		exit(1);
	}

	bzero(sdbuf, LENGTH); 
	int fs_block_sz; 
	while((fs_block_sz = fread(sdbuf, sizeof(char), LENGTH, fs)) > 0)
	{
		if(send(sd, sdbuf, fs_block_sz, 0) < 0)
		{
		    fprintf(stderr, "ERROR: Failed to send file %s. (errno = %d)\n", fs_name, errno);
		    break;
		}
		bzero(sdbuf, LENGTH);
	}
	printf("Ok File %s from Client was Sent!\n", fs_name);


	close (sd);
	printf("[Client] Connection lost.\n");
	return (0);
}
