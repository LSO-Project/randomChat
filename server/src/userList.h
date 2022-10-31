#include "user.h"

//Struttura lista
struct UserNode {
	User* user;
	struct UserNode* next;
};

typedef struct UserNode* UserList;

//Funzioni per lista
UserList initUserNode(User* user);
UserList appendUserNode(UserList L, User* user);
UserList removeUserNode(UserList L, User* target);

int checkUserInList(UserList L, User* user);  //Verifica se un'utente è presente nella lista oppure no
User* findOpponent(UserList L, User* previusMatch);  //Ottiene il primo utente da L che è diverso da previusMatch