#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "userList.h"

UserList initUserNode(User* user) {
    UserList L = (UserList)malloc(sizeof(struct UserNode));
    L->user = user;
    L->next = NULL;
    return L;
}

UserList appendUserNode(UserList L, User* user) {
    if (L != NULL) 
        L->next = appendUserNode(L->next, user);
    else
        L = initUserNode(user);
    return L;
}

UserList removeUserNode(UserList L, User* target) {
    if (L != NULL) {
        if (L->user == target) {
            UserList tmp = L->next;
            free(L);
            return tmp;
        }
        L->next = removeUserNode(L->next, target);
    }
    return L;
}

int checkUserInList(UserList L, User* user){
    if (L != NULL){
        if(L->user == user)
            return 1;
        else
            return checkUserInList(L->next,user);
    }else
        return 0;
}

User* findOpponent(UserList L, User* previusMatch){
    if(L != NULL){
        if(L->user != previusMatch)  //Verifica se l'utente è diverso dall'ultimo match
            return L->user;
        else
            return findOpponent(L->next, previusMatch);
    }else
        return NULL;
}
