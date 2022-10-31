#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "server.h"

int main(int argc, char *argv[]) {

	char* SOCK_ADDRESS = "127.0.0.1";
  	int SOCK_PORT = 5200;
  	int chatLife = 2;
  	char* pathRoomsFile = NULL;

	if((argc == 1)||(argc == 3)||(argc == 5)||(argc == 7)||(argc == 9)){

		for(int i = 1; i < argc; i=i+2){

    		if(strcmp(argv[i], "-i") == 0)
      			SOCK_ADDRESS = argv[i+1];

    		if(strcmp(argv[i], "-p") == 0)
      			SOCK_PORT = atoi(argv[i+1]);

    		if(strcmp(argv[i], "-t") == 0)
      			chatLife = atoi(argv[i+1]);

    		if(strcmp(argv[i], "-r") == 0)
      			pathRoomsFile = argv[i+1];

  		}
	}
 	
	else{

		if(argc == 2 && strcmp(argv[1], "--help")==0){
			printf("Usage: ./Server [options]\n");
			printf("Options:\n");
			printf("-i <address>\tImposta l'indirizzo su cui viene avviato il server.\n");
			printf("-p <port>\tImposta la porta da usare\n");
			printf("-t <time>\tImposta la durate di una chat, <time> è espresso in minuti.\n");
			printf("-r <file>\tImposta il file da cui verranno caricare le room.\n");
			printf("\n");
			printf("Tutti i campi [options] non sono necessari per il funzionamento del programma,\n");
			printf("di default assumono i seguenti valori:\n");
			printf("address=127.0.0.1\tport=5200\ttime=2\n");
			printf("Verrà sempre creata una room di default chiamata Main.\n");
			return 1;
		}else{
			printf("Invocazione non corretta.\nUsare ./Server --help per ulteriori informazioni.\n");
			return -1;
		}
	}
	
	createServer(SOCK_ADDRESS,SOCK_PORT, chatLife);
  	loadRooms(pathRoomsFile);
	startServer();

	return 0;
	
}