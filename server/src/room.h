#include <pthread.h>
#include "userList.h"
#include "chatList.h"

typedef struct RoomStruct {

	char* roomName;

	pthread_mutex_t users_mutex;
	int usersInRoomNum;  //Numero totale utenti nella stanza
	int pendingUsersNum;  //Numero utenti in attesa
    UserList headPendingUsers;  //Testa lista utenti in attesa

	pthread_mutex_t activeChat_mutex;
	int acticeChatNum;
	ChatList headActiveChat;  //Testa lista chat attive nella room
	
} Room;

Room* createRoom(char* name);
void addUserToRoom(Room* room, User* user);  //Aggiunge utente a stanza
void addUserToRoomPending(Room* room, User* user);  //Aggiunge un'utente a una stanza (aggiunto a pending)
void removeUserFromRoomPending(Room* room, User* user);  //Rimuove utente da una stanza (rimuove da pending)
void removeUserFromRoom(Room* room, User* user);  //Rimuove utente da stanza
void addChatToRoom(Room* room, Chat* chat);
void removeChatFromRoom(Room* room, Chat* chat);