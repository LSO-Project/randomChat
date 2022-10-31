#include "user.h"

typedef struct ChatStruct {
	char idChat[256];  //Identificativo chat, unico per ogni chat, viene ottenuto dalla concatenazione del nickname del primo e del secondo utente
    User* user1;
    User* user2;
} Chat;

Chat* createChat(User* user1, User* user2);