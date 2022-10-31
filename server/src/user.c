#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "user.h"

User* createUser(void *socketDescriptor){
    User* user =  (User*)malloc(sizeof(User));  // Alloco memoria per nuovo utente
    if (user == NULL)
        return NULL;

    user->socketDescriptor = *(int*)socketDescriptor;

    strcpy(user->internalCommand,"noCommand");

    return user;
}