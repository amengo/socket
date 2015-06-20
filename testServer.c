void *mezzoCondiviso(void * arg){
	/**********************************************************************************/
	/*Thread relativo al mezzo condiviso.Crea socketserver,fa operazioni d´invio frame*/
	/**********************************************************************************/
	int err,i,j, OptVal, maxFD,nReady,nLetti, nTerminati=0, probErrore;
	int fdMittente; /*fd dal quale abbiamo ricevuto il frame*/
	int collisione=0;
	unsigned int lungh;
	int socketfd, newsocketfd[6], termina[6];
	struct sockaddr_in Server, ClientS[6] ;
	frame FServer;
	struct timeval tempoAttuale;
	fd_set readFD;
	buffer *primoBuf=NULL;
	buffer *scorriBuf,*scorriBufAppoggio;
	char* punt;
	/**********************************************************************************/
	/*             			 Creiamo il socket  	      		          */
	/**********************************************************************************/
	socketfd = socket(AF_INET, SOCK_STREAM, 0);
	if (socketfd == SOCKET_ERROR) {
		printf ("socket() failed, Err: %d \"%s\"\n", errno,strerror(errno));
		exit(1);
	}
	/**********************************************************************************/
	/*   	       	   Settiamo gli indirizzi e le porte del server                   */
	/**********************************************************************************/
	memset ( &Server, 0, sizeof(Server) );
	Server.sin_family = AF_INET;
	Server.sin_addr.s_addr = inet_addr("127.0.0.1");
	Server.sin_port = htons(34567);

	/**********************************************************************************/
	/*    Settiamo le option per poter riutilizzare l´address se è già occupato       */
	/**********************************************************************************/
	OptVal = 1; /*reuseaddr*/
	err = setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR , (char *)&OptVal, sizeof(OptVal));

	if (err == SOCKET_ERROR)  {
		printf ("setsockopt() SO_REUSEADDR failed, Err: %d \"%s\"\n", errno,strerror(errno));
		exit(1);
	}

	/**********************************************************************************/
	/*              		Facciamo la bind     		                  */
	/**********************************************************************************/
	err = bind(socketfd, (struct sockaddr*) &Server, sizeof(Server));
	if (err == SOCKET_ERROR)  {
		printf ("bind() failed, Err: %d \"%s\"\n",errno,strerror(errno));
		exit(1);
	}

	/**********************************************************************************/
	/*            	        	Facciamo la listen  			          */
	/**********************************************************************************/
	err = listen(socketfd, 1 );
	if (err == SOCKET_ERROR)  {
		printf ("listen() failed, Err: %d \"%s\"\n",errno,strerror(errno));
		exit(1);
	}

	/**********************************************************************************/
	/* Facciamo le accept per i client,e stiamo fermi finchè non sono tutti collegati */
	/**********************************************************************************/
	memset ( &ClientS, 0, sizeof(ClientS) );
	lungh= sizeof(ClientS[i]);
	newsocketfd = accept(socketfd, (struct sockaddr*) &ClientS, &lungh);
	if ((newsocketfd<0)&&(errno==EINTR)){
		printf ("errore nell'accept, Err: %d \"%s\"\n",errno,strerror(errno));
	}

	if (newsocketfd == SOCKET_ERROR)  {
		printf ("accept() failed, Err: %d \"%s\"\n",errno,strerror(errno));
		exit(1);
	}


	/**********************************************************************************/
	/*      		    Settiamo e facciamo la select    		          */
	/**********************************************************************************/



	while(1){ 
		FD_ZERO(&readFD);
		maxFD=0;
		nReady=0;      

		FD_SET(newsocketfd,&readFD);
		if(newsocketfd>maxFD){
			maxFD=newsocketfd;
		}
		 


		nReady=select(maxFD+1,&readFD,NULL,NULL,NULL); /* dice quanti messaggi mi sono arrivati */

		nLetti=recv(newsocketfd,&FServer , sizeof(frame), 0); 
		FD_CLR(newsocketfd,&readFD);
		break;

				else if(nLetti>0){ /* cerca un buffer da riempire, e lo riempie con i dati arrivati */
					scorriBuf=trovaBuffer(socketfd, primoBuf, &FServer,nLetti );     
					if(primoBuf==NULL){
						primoBuf=scorriBuf;
					}
					scorriBuf->fdMitt=fdMittente;
				}

			}
		}
		 



	/**********************************************************************************/
	/*       		       Chiudiamo tutti i socket     	   		  */
	/**********************************************************************************/
	for(i=0;i<6;i++){
		close(newsocketfd);
	}

	close(socketfd);
	printf("fine\n");
	pthread_exit (NULL);
}
 

