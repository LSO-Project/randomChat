
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <sys/socket.h>
#include <string.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <netinet/in.h>
#include <arpa/inet.h>

#include "server.h"
#include "log.h"
#include "signals.h"
#include "connection.h"

Server* server;

void addUser(User* user){
  sysLog("[+] Aggiungo nuovo utente a server");
  pthread_mutex_lock(&server->users_mutex);
  server->headUserList = appendUserNode(server->headUserList, user);  //Aggiunge utente a lista utenti
  server->connectedUsers++;
  pthread_mutex_unlock(&server->users_mutex);
}  

void removeUser(User* user){
  sysLog("[-] Rimuovo utente da server");
  pthread_mutex_lock(&server->users_mutex);
  server->headUserList = removeUserNode(server->headUserList, user);  //Rimuove utente da lista utenti
  server->connectedUsers--;
  pthread_mutex_unlock(&server->users_mutex);
}

void addRoom(Room* room){
  sysLog("[+] Aggiungo stanza %s al server",room->roomName);
  pthread_mutex_lock(&server->rooms_mutex);
  server->headRoomList = appendRoomNode(server->headRoomList, room);  //Aggiunge stanza a lista stanze
  server->activeRooms++;
  pthread_mutex_unlock(&server->rooms_mutex);
} 

void removeRoom(Room* room){
  sysLog("[-] Rimuove stanza da server");
  pthread_mutex_lock(&server->rooms_mutex);
  //Disconnette dalla stanza tutti gli utenti connessi
  server->headRoomList = removeRoomNode(server->headRoomList, room);  //Rimuove stanza da lista stanze
  server->activeRooms--;
  pthread_mutex_unlock(&server->rooms_mutex);
}

void createServer(char* address, int port, int chatLife){

  server = (Server*)malloc(sizeof(Server));
  server->connectedUsers = 0;
  server->headUserList = NULL;
  server->activeRooms = 0;
  server->headRoomList = NULL;
  server->chatLifeC = (char*)malloc(sizeof(char)*10);
  setChatLife(chatLife);

  addRoom(createRoom("Main"));  //Crea la stanza di default

  server->address = address;
  server->port = port;
} 

int loadRooms(char* pathRoomsFile){
  if(pathRoomsFile != NULL){

    FILE * fp;
    char * line = NULL;
    size_t len = 0;
    ssize_t read;

    fp = fopen(pathRoomsFile, "r");
    if (fp == NULL)
        return -1;

    while ((read = getline(&line, &len, fp)) != -1) {
        line[strcspn(line, "\r\n")] = 0;
        addRoom(createRoom(line));
    }

    fclose(fp);
    if (line)
        free(line);

  }else{
    return -1;
  }
  return 1;
}

void setChatLife(int time){
  server->chatLife = time;
  sprintf(server->chatLifeC,"%d",time);
}

int redirect(User* user, User* opponent){

  sysLog("[+] Avvio funzione redirect da: %s -> %s",user->nickname, opponent->nickname);

  while(checkConnection(user)){

    char* msg = reciveMessage(user);

    //L'utente esce dalla chat
    if(strcmp(msg,"res:userLeave")==0){
      sendMessage("cmd:userLoggedOut", opponent);  //Avvisa l'opponent che user ha abbandonato la chat (Usando questo sistema è immediata la comunicazione di uscita)
      return 0;
    }

    //L'opponent ha abbandonato
    if(strcmp(msg,"res:opponentLeave")==0){
      return 1;
    }

    //L'user richiede una nuova chat
    if(strcmp(msg,"res:userRequestNewChat")==0){
      sendMessage("cmd:userLoggedOut", opponent);  //Avvisa l'opponent che user ha abbandonato la chat (Usando questo sistema è immediata la comunicazione di uscita)
      return 1;
    }

    //Il tempo del client è terminato
    if(strcmp(msg,"res:timeExpired")==0){
      return 2;
    }

    sendMessage(msg, opponent);  //Invia messaggio di user a opponent
    free(msg);
  }
  
  return -1;  //Disconnessione improvvisa del client

}

Chat* match(Room* room, User* user){

  User* opponent = findOpponent(room->headPendingUsers, user->previusMatch);  //Cerca utente dalla pending a cui connettersi escludendo i collegamenti precedenti

  //Se trova utente valido
  if(opponent != NULL){
    
    removeUserFromRoomPending(room,opponent);  //Rimuove l'opponent dalla pending (fare testing)
    user->previusMatch = opponent;  //Setta l'opponent come ultimo match
  
    Chat* chat = createChat(user,opponent);  //Crea istanza di chat
    addChatToRoom(room,chat);  //Aggiunge chat a room
    
    strcpy(opponent->internalCommand,"chatCreated");  //Indica all'opponent che la chat è stata creata
    strcpy(opponent->internalData,chat->idChat);  //Indica l'id della chat che è stat creata all'opponent
    
    sendMessage("res:opponentFound", user);  //Avvisa l'utente che è stato trovato l'opponent
    sendMessage(opponent->nickname, user);  //Invia nickname opponent

    return chat;

  }else{

    //Non è stato trovato un'opponent

    addUserToRoomPending(room,user);  //Aggiungo user a pending

    //Finchè l'utente è connesso
    while(checkConnection(user)){

      //Controlla se client decide di abbandonare pending
      if(checkMessage(user) == 1){
        char* msg = reciveMessage(user);
        if(strcmp(msg,"cmd:leavePendingList") == 0){
          sysLog("[-] %s decide di abbandonare l'attesa",user->nickname);  //Si è verificato un' errore sconosciuto
          removeUserFromRoomPending(room,user);  //Rimuove l'utente dalla pending (fare testing)
          return NULL;
        }
      }

      //E' stata creata una chat
      if(strcmp(user->internalCommand,"chatCreated") == 0){
        strcpy(user->internalCommand,"noCommand");  //Resetta comando interno
        Chat* chat = getChatById(room->headActiveChat,user->internalData);  //Ricavo istanza chat tramite id
        if(chat == NULL){
          sysLog("[!] Si è verificato un'errore durante la creazione dalla chat");  //Si è verificato un' errore sconosciuto
        }
        else{
          sysLog("[+] E' stata creata una chat tra %s e %s",chat->user1->nickname,chat->user2->nickname);
          opponent = chat->user1;  //Setto l'opponent (user1 perché è stato l'opponent a creare la chat)
          sendMessage("res:opponentFound", user);  //Avvisa l'utente che è stato trovato l'opponent
          sendMessage(opponent->nickname, user);  //Invia nickname opponent
          user->previusMatch = opponent;  //Setta l'opponent come ultimo match
        }

        return chat;

      }

      sleep(1);
      
    }

    removeUserFromRoomPending(room,user);  //Rimuove l'utente dalla pending (fare testing)
  }
  return NULL;
}

void chatController(Room* room, User* user){

  Chat* chat = match(room,user);  //Avvia funzione di match (trova opponent e crea istanza chat)
  if(chat == NULL){
    return;
  }

  User* opponent = NULL;
  if(user == chat->user1)
    opponent = chat->user2;
  else
    opponent = chat->user1;

  int chatRes = redirect(user,opponent);  //Avvia funzione di redirect dei messaggi per la chat

  //Rimuovo istanza di chat (Il creatore della chat rimuove l'istanza)
  if(user == chat->user1)
    removeChatFromRoom(room,chat);

  //Casi terminazione chat

  //Il client si è disconnesso improvvisamente
  if(chatRes == -1){
    sendMessage("cmd:userLoggedOut", opponent);  //Avvisa l'opponent che user ha abbandonato
  }

  //Chat terminata per uscita utente
  if(chatRes == 0){
    return;
  }

  //Chat terminata per uscita opponent oppure user richiede nuova chat
  if(chatRes == 1){
    chatController(room, user);  //Avvia nuova chat
  }

  //Chat terminata per tempo
  if(chatRes == 2){
    chatController(room, user);  //Avvia nuova chat
  }
  
}

void roomConnect(Room* room, User* user){
    sysLog("[+] L'utente: %s è entrato nella room: %s",user->nickname,room->roomName);
    addUserToRoom(room,user);  //Aggiunge utente a stanza
    chatController(room,user);  //Avvia funzione di match
    removeUserFromRoom(room, user);  //Rimuovo utente da stanza
}

int startServer(){

  sysLog("[+] Avvio server");
  bindSignals();

  int fd, conn_fd;
  
  if((fd = socket(PF_INET, SOCK_STREAM, 0)) < 0)
    perror("[!] Socket error"),exit(1);
  
  struct sockaddr_in indirizzo;
  indirizzo.sin_family = AF_INET;
  indirizzo.sin_port = htons(server->port);

  if(inet_aton(server->address,&indirizzo.sin_addr) == 0)
    perror("[!] inet_aton"),exit(1);
  memset(&(indirizzo.sin_zero),'\0',8);
  
  if(bind(fd, (struct sockaddr*)&indirizzo, sizeof(indirizzo)) < 0)
    perror ("[!] Bind error"),exit(1);
  
  if(listen(fd,5) < 0)
    perror("[!] Listen error"),exit(1);
  
  //Struttura per client
  struct sockaddr_in client_addr;
  socklen_t slen = sizeof(client_addr);

  pthread_t tid;
  
  while((conn_fd = accept(fd,(struct sockaddr *)&client_addr, &slen))){
    
    struct sockaddr_in* pV4Addr = (struct sockaddr_in*)&client_addr;
    struct in_addr ipAddr = pV4Addr->sin_addr;
    char str[INET_ADDRSTRLEN];
    inet_ntop( AF_INET, &ipAddr, str, INET_ADDRSTRLEN ); 

    User* user = createUser(&conn_fd);  //Crea nuovo user
    addUser(user);  //Aggiunge user a lista

    sysLog("[+] Client connesso -> IP: %s SD: %d ",str,user->socketDescriptor);
    
    pthread_create(&tid,NULL, clientController, user);
   
  }
  
  sysLog("[!] Chiusura server");
  close(fd);
  
  return 0;
}

void* clientController(void* val){

  User* user = (User*)val;

  while(checkConnection(user)){
    
    char* msg = reciveMessage(user);
   
    //L'utente setta il nickname
    if(strcmp(msg,"cmd:setNickname")==0){
      sysLog("[+] Richiesta setNickname");
      user->nickname = reciveMessage(user);
      sysLog("[+] Client %d setta nickname : %s",user->socketDescriptor,user->nickname);
      sendMessage("res:nicknameSetted", user);  //Invia messaggio al client avvisandolo che il nickname è stato settato
    }

    //L'utente richiede lista stanze
    else if(strcmp(msg,"cmd:getRooms")==0){
      sysLog("[+] Utente richiede lista stanze");
      char* roomsList = getRoomList(server->headRoomList);
      sendMessage(roomsList, user);  //Invia lista stanze
    }

    //L'utente si connette a stanza
    else if(strcmp(msg,"cmd:connectTo")==0){

      sysLog("[+] L'utente %s #%d richiede di connettersi a stanza",user->nickname,user->socketDescriptor);
      char* data = reciveMessage(user);  //Riceve messaggio con id stanza a cui vuole collegarsi
      int roomNum = atoi(data);
      Room* room = getRoom(server->headRoomList,roomNum);  //Ricava stanza da id
      if(room != NULL){
        sendMessage("res:startConnect", user);  //Avvisa l'utente che è iniziata la fase di connessione
        sendMessage(server->chatLifeC, user);  //Invia durata chat a utente
        roomConnect(room,user);  //Ingresso in room
      }else{
        sendMessage("res:roomNotExist", user);  //Avvisa l'utente che la stanza richiesta non esiste
      }
    }

    else{
      sysLog("[+] L'utente %s #%d invia un comando non valido",user->nickname,user->socketDescriptor);
    }

    free(msg);
  }

  sysLog("[-] Client con socketDescriptor: %d disconnesso", user->socketDescriptor);

  removeUser(user);  //Rimuove utente disconnesso
  close(user->socketDescriptor);

  return NULL;
}