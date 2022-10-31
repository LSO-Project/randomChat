#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

#include "room.h"
#include "connection.h"
#include "lobby.h"

int chatLife;

void roomController(){
    printw("Ingresso nella room\n");

    //char* res = reciveMessage();  //Ottiene durata chat
    //int chatLife = atoi(res);
    chatLife = atoi(reciveMessage());   //Ottiene durata chat
    lobbyController(chatLife);
}