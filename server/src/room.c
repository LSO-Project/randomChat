#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <string.h>
#include "room.h"

Room* createRoom(char* name){
    Room* room = (Room*)malloc(sizeof(Room));
    room->roomName = (char*)malloc(sizeof(char)*256);
    strcpy(room->roomName,name);
    room->usersInRoomNum = 0;
    room->pendingUsersNum = 0;
    room->headPendingUsers = NULL;
    room->acticeChatNum = 0;
    room->headActiveChat = NULL;
    return room;
}

void addUserToRoom(Room* room, User* user){
    pthread_mutex_lock(&room->users_mutex);
    room->usersInRoomNum++;
    pthread_mutex_unlock(&room->users_mutex);
}

void removeUserFromRoom(Room* room, User* user){
    pthread_mutex_lock(&room->users_mutex);
    room->usersInRoomNum--;  //Decremento utenti totali su room
    pthread_mutex_unlock(&room->users_mutex);
}

void addUserToRoomPending(Room* room, User* user){
    pthread_mutex_lock(&room->users_mutex);
    room->headPendingUsers = appendUserNode(room->headPendingUsers, user);  //Aggiunge utente a lista utenti pending
    room->pendingUsersNum++;
    pthread_mutex_unlock(&room->users_mutex);
}

void removeUserFromRoomPending(Room* room, User* user){
    pthread_mutex_lock(&room->users_mutex);
    room->headPendingUsers = removeUserNode(room->headPendingUsers, user);  //Aggiunge utente a lista utenti pending
    room->pendingUsersNum--;  //Decremento utenti in pending
    pthread_mutex_unlock(&room->users_mutex);
}

void addChatToRoom(Room* room, Chat* chat){
    pthread_mutex_lock(&room->activeChat_mutex);
    room->headActiveChat = appendChatNode(room->headActiveChat, chat);  //Aggiungo chat a lista chat
    room->acticeChatNum++;  //Incremento numero chat attive
    pthread_mutex_unlock(&room->activeChat_mutex);
}

void removeChatFromRoom(Room* room, Chat* chat){
    pthread_mutex_lock(&room->activeChat_mutex);
    room->headActiveChat = removeChatNode(room->headActiveChat, chat);
    room->acticeChatNum--;  //Decremento numero chat attive su room
    pthread_mutex_unlock(&room->activeChat_mutex);
}