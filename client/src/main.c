#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <errno.h>
#include <ncurses.h>

#include "menu.h"
#include "connection.h"

int main(int argc, char *argv[]){

  initscr();
  cbreak();
  keypad(stdscr, TRUE);
  
  const char *SOCK_ADDRESS = "127.0.0.1";
  int SOCK_PORT = 5200;

  if((argc == 1)||(argc == 3)||(argc == 5)){

		for(int i = 1; i < argc; i=i+2){

    		if(strcmp(argv[i], "-i") == 0)
      			SOCK_ADDRESS = argv[i+1];

    		if(strcmp(argv[i], "-p") == 0)
      			SOCK_PORT = atoi(argv[i+1]);
  		}
	}
 	
	else{

		if(argc == 2 && strcmp(argv[1], "--help")==0){
			printw("Usage: ./Server [options]\n");
			printw("Options:\n");
			printw("-i <address>\tImposta l'indirizzo del server a cui connettersi.\n");
			printw("-p <port>\tImposta la porta del server a cui connettersi\n");
			printw("Tutti i campi [options] non sono necessari per il funzionamento del programma,\n");
			printw("di default assumono i seguenti valori:\n");
			printw("address=127.0.0.1\tport=5200\n");
      refresh();
      endwin();
			return 1;
		}else{
			printw("Invocazione non corretta.\nUsare ./Client --help per ulteriori informazioni.\n");
      refresh();
      endwin();
			return -1;
		}

	}
  
  if((sd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    perror("[!] Socket Error"),exit(1);
    
  struct sockaddr_in mio_indirizzo;
  mio_indirizzo.sin_family = AF_INET;
  mio_indirizzo.sin_port = htons(SOCK_PORT);
  inet_aton(SOCK_ADDRESS,&mio_indirizzo.sin_addr);
  
  if(connect(sd, (struct sockaddr*)&mio_indirizzo, sizeof(mio_indirizzo)) < 0){
    printw("[!] Connessione al server fallita");
    refresh();
    endwin();
    exit(1);

  }else
    printw("[+] Connessione al server stabilita\n");
  
  menuLoginClient();
  
  //Termina connessione
  close(sd);
  printw("[+] Connessione al server terminata\n");
  refresh();
  
  endwin();
  
  return 0;
}