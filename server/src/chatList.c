#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "chatList.h"

ChatList initChatNode(Chat* chat) {
    ChatList L = (ChatList)malloc(sizeof(struct ChatNode));
    L->chat = chat;
    L->next = NULL;
    return L;
}

ChatList appendChatNode(ChatList L, Chat* chat) {
    if (L != NULL) 
        L->next = appendChatNode(L->next, chat);
    else
        L = initChatNode(chat);
    return L;
}

ChatList removeChatNode(ChatList L, Chat* target) {
    if (L != NULL) {
        if (L->chat == target) {
            ChatList tmp = L->next;
            free(L);
            return tmp;
        }
        L->next = removeChatNode(L->next, target);
    }
    return L;
}

Chat* getChatById(ChatList L, char* idChat){
    if (L != NULL) {
        if(strcmp(L->chat->idChat,idChat) == 0)
            return L->chat;
        else
            return getChatById(L->next,idChat);
    }else
        return NULL;
}

Chat* checkUserInChatList(ChatList L, User* user){
    if (L != NULL) {
        if((L->chat->user1 == user) || (L->chat->user2 == user))
            return L->chat;
        else
            return checkUserInChatList(L->next,user);
    }
    return NULL;
}