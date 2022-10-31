#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <ncurses.h>

#include "menu.h"
#include "connection.h"
#include "room.h"

char* myNickname = NULL;

void menuStanze(){

    int flag = 1;
    while(flag){

        clear();

        printw("Lista stanze:\n");
        sendMessage("cmd:getRooms");    //Richiede elenco stanze attive
        char* roomList = reciveMessage();  //Riceve lista stanze
        printw("%s\n",roomList);  //Print stanze 
        free(roomList);

        printw("Cosa vuoi fare?\n1) Connetti a stanza\n2) Aggiorna stanze\n3) Indietro\nLa tua scelta:");
        int chose;
        scanw("%d",&chose);
    
        switch(chose){
            case 1:
                printw("[+] L'utente seleziona 'Connetti a stanza'\n");
                char roomId[6];
                memset(roomId,0,sizeof(roomId));
                printw("ID Stanza alla quale ti vuoi connettere: ");
                getstr(roomId);
                sendMessage("cmd:connectTo");  //Invia richiesta di connessione a stanza
                sendMessage(roomId);  //Invia numero corrispondente a stanza

                char* res = reciveMessage();  //Riceve risposta
                if(strcmp(res,"res:startConnect") == 0)
                    roomController();
                else if(strcmp(res,"res:roomNotExist") == 0)
                    printw("[!] L'id della room inserito non è valido\n");
                else
                    printw("[!] Errore durante ingersso in room\n");
                free(res);
                break;
            case 2:
                printw("[+] L'utente seleziona aggiorna stanze\n");
                break;
            case 3:
                printw("[+] L'utente seleziona indietro\n");
                flag = 0;
                break;
            default:
                printw("[!] L'utente fa una scelta non valida\n");
                menuStanze();
                break;
        }     

        nodelay(stdscr, FALSE);
    }
}

void menuPrincipale(){

    int flag = 1;
    while(flag){
        clear();

        int chose;
        printw("Cosa vuoi fare?\n1) Connetti a stanza\n2) Esci\nLa tua scelta:");
        scanw("%d",&chose);
    
        // Eseguo controllo sull'input 
        switch (chose){
            case 1: //Entra
                printw("[+] L'utente seleziona 'Connetti a stanza'\n");
                menuStanze();
                break;
            case 2:  //Esci
                printw("[+] L'utente seleziona 'Esci'\n");
                flag = 0;
                break;
            default:
                printw("[!] L'utente fa una scelta non valida\n");
                break;
        }
    }
  
}

int menuInserisciNickname(){

    clear();

    char* buffer = (char*)malloc(sizeof(char)*256);
    memset(buffer,0,sizeof(char)*256);
    
    printw("Inserire il proprio nickname : ");
    //scanw("%s",buffer);
    getstr(buffer);
    printw("[+] Invio a server richiesta settaggio nickname\n");

    sendMessage("cmd:setNickname");  //Invio a server richiesta settaggio nickname
    sendMessage(buffer);  //Invio nickname a server

    char* conferma = reciveMessage();  //Attendo conferma dal server
    if(strcmp(conferma,"res:nicknameSetted") == 0){
        printw("[+] Nickname settato correttamente\n");
        myNickname=buffer;
        menuPrincipale();
        return 0;
    }else{
        printw("[!] Errore durante il settaggio del nickname\n");
        return 2;
    }

}

void menuLoginClient(){
    
    int chose;
    printw("_____BENTORNATO!!_____\nCosa vuoi fare?\n1) Entra nella chat\n2) Esci\nLa tua scelta:");
    scanw("%d",&chose);

    switch (chose){
        case 1: //Entra
            printw("[+] L'utente seleziona entra nella chat\n");
            if(menuInserisciNickname() == 2)
                menuLoginClient();  //Errore durante il settaggio del nickname
            break;
        case 2: //Esci
            printw("[+] L'utente seleziona esci\n");
            break;
        default:
            printw("Scelta non valida\n");
            menuLoginClient();
            break;
  }

}