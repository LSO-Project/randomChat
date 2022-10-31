#include <stdio.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/un.h>
#include <errno.h>

#include <netinet/in.h>
#include <arpa/inet.h>

#include "connection.h"

int sd;

void sendMessage(char* message){

  int msgLen = strlen(message);  
  char* buffer = (char*)malloc(sizeof(char)*(msgLen + 12));
  memset(buffer,0,(msgLen+12));
  sprintf(buffer,"%d#%s",msgLen,message);
  strcat(buffer,"\0");

  write(sd, buffer, strlen(buffer));  //Invia messaggio

  free(buffer);
  
}

char* reciveMessage(){

  char* msgLenC = (char*)malloc(sizeof(char)*6);
  memset(msgLenC,0,6);
  char tmp;
  while(recv(sd,&tmp,1, 0) && tmp!='#')
    strncat(msgLenC,&tmp,1);

  int msgLen = atoi(msgLenC);  //Converte da char a int

  char* buffer = (char*)malloc(sizeof(char)*msgLen);
  memset(buffer,0,msgLen);
  recv(sd,buffer,msgLen, 0);
  buffer[msgLen] = '\0';

  return buffer;
}

int checkConnection(){

  char data;
  int retval = recv(sd,&data,1, MSG_PEEK | MSG_DONTWAIT);  //Legge un byte senza rimuoverlo (MSG_PEEK) e non è bloccante (MSG_DONTWAIT)
  if(retval != 0)
    return 1;
  else
    return 0;  

}

int checkMessage(){

  char data;
  int retval = recv(sd,&data,1, MSG_PEEK | MSG_DONTWAIT);  //Legge un byte senza rimuoverlo (MSG_PEEK) e non è bloccante (MSG_DONTWAIT)
  return retval;  

}