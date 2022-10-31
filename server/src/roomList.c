#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "roomList.h"

RoomList initRoomNode(Room* room) {
    RoomList L = (RoomList)malloc(sizeof(struct RoomNode));
    L->room = room;
    L->next = NULL;
    return L;
}

RoomList appendRoomNode(RoomList L, Room* room) {
    if (L != NULL) 
        L->next = appendRoomNode(L->next, room);
    else
        L = initRoomNode(room);
    return L;
}

RoomList removeRoomNode(RoomList L, Room* target) {
    if (L != NULL) {
        if (L->room == target) {
            RoomList tmp = L->next;
            free(L);
            return tmp;
        }
        L->next = removeRoomNode(L->next, target);
    }
    return L;
}

Room* getRoom(RoomList L, int i){
    if(i == 1)
        return L->room;
    if(L != NULL && i>0)
        return getRoom(L->next,--i);
    return NULL;
}

char* getRoomList(RoomList L){
    RoomList TmpHead = L;

    char* buffer = (char*)malloc(sizeof(char)*1024);
    memset(buffer,0,1024);
    
    if(TmpHead == NULL)
        return NULL;

    for(int i = 1; TmpHead != NULL; i++, TmpHead = TmpHead->next){
        sprintf(buffer, "%sid: %d \t Nome: %s \t Utenti totali connessi: %d \t Utenti in attesa: %d\n", buffer, i,TmpHead->room->roomName,TmpHead->room->usersInRoomNum,TmpHead->room->pendingUsersNum);
    }
    
    return buffer;
}

Chat* removeUserFromRoomList(RoomList L, User* user){
    if (L != NULL) {
        if((checkUserInList(L->room->headPendingUsers,user) == 1)){
            removeUserFromRoom(L->room,user);  //Rimuove utente da room
            return NULL; //Ritorna null'perché l'utente si trova nella pending
        }
        //Verifica se si trova in una chat di quella stanza
        Chat* chat = checkUserInChatList(L->room->headActiveChat,user);
        if(chat != NULL)
            return chat;

        return removeUserFromRoomList(L->next,user);
    }
    return NULL;
}