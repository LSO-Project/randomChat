#include "room.h"

//Struttura lista
struct RoomNode {
	Room* room;
	struct RoomNode* next;
};

typedef struct RoomNode* RoomList;

//Funzioni per lista
RoomList initRoomNode(Room* room);
RoomList appendRoomNode(RoomList L, Room* room);
RoomList removeRoomNode(RoomList L, Room* target);

char* getRoomList(RoomList L);  //Ottiene lista stanze più info per ogni stanza
Room* getRoom(RoomList L, int i); //Ottiene room da int altrimenti ritorna null

Chat* removeUserFromRoomList(RoomList L, User* user);  //Rimuove utente dalla room in cui si trova, se l'utente si trova in una chat ritorna la chat