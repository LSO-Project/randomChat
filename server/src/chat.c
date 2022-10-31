#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chat.h"

Chat* createChat(User* user1, User* user2){
    Chat* chat = (Chat*)malloc(sizeof(Chat));
    chat->user1 = user1;
    chat->user2 = user2;
    strcpy(chat->idChat,user1->nickname);
    strcat(chat->idChat,user2->nickname);
    return chat;
}