#include <pthread.h>
#include "roomList.h"
 
typedef struct ServerStruct {

	char* address;
	int port;

	int chatLife;	//Durata chat (espresso in min)
	char* chatLifeC;

	pthread_mutex_t users_mutex;
	int connectedUsers; //Numero totale di utenti connessi
	UserList headUserList;  //Testa lista per gestione utenti connessi

	pthread_mutex_t rooms_mutex;
	int activeRooms;  //Numero room attive
	RoomList headRoomList;  //Lista per gestire le stanze
	
} Server;

void setChatLife(int time);

void addUser(User* user);  //Aggiunge utente a lista utenti connessi
void removeUser(User* user);  //Rimuove utente da lista utenti connessi
void addRoom(Room* room);  //Aggiunge stanza
void removeRoom(Room* room);  //Rimuove stanza

int redirect(User* user, User* opponent);  //Funzione che reindirizza i messaggi da user a opponent
Chat* match(Room *room, User *user);  //Funzione di match
void chatController(Room* room, User* user);
void roomConnect(Room* room, User* user);  //Funzioni di room

void createServer(char* address, int port, int chatLife);
int loadRooms(char* pathRoomsFile);  //Aggiunge le room presenti in pathRoomsFile
int startServer();
void* clientController(void* val);