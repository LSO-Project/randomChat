#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ncurses.h>

#include "lobby.h"
#include "connection.h"
#include "chat.h"

char* opponentNickname;

int match(){

    clear();
    printw("[+] Avvio funzione Match\n");
    printw("Sei in attesa di un collegamento...\n");
    printw("Premere invio per abbandonare l'attesa\n");
    refresh();

    int bMatch = 0;

    nodelay(stdscr, TRUE);  //Disabilita input bloccante
    int ch;
    
    while(checkConnection() && bMatch == 0){

        if(checkMessage() == 1){
            char* res = reciveMessage();

            //Caso in cui è il nostro thread lato server a trovare l'opponent
            if(strcmp(res,"res:opponentFound") == 0){
                opponentNickname = reciveMessage();  //Attende opponent nickname
                return 1;
            }
        }

        //Controllo uscita
        ch = getch();
        if (ch == '\n')
            bMatch=1;

        napms(500);
    }

    nodelay(stdscr, FALSE);
    
    if(bMatch == 1){
        sendMessage("cmd:leavePendingList");
    }

    return 0;
}

void lobbyController(int chatLife){

    if(match()){    //Avvia funzione match
        printw("[+] Opponent trovato, nickname: %s\n",opponentNickname);

        int exitCode = chat();
        free(opponentNickname); //new
        
        if(exitCode == -1){
            printw("[!] Connessione con il server terminata\n");
            return;
        }

        if(exitCode == 0){
            printw("[-] Chat terminata\n");
            return;
        } 
        
        if(exitCode == 1){
            printw("[-] L'opponent ha abbandonato la chat\n");
            printw("[+] Avvio nuova chat\n");
            lobbyController(chatLife);
            return;
        }

        if(exitCode == 2){
            printw("[+] Avvio nuova chat\n");
            lobbyController(chatLife);
            return;
        }

        if(exitCode == 3){
            printw("[+] Avvio nuova chat\n");
            lobbyController(chatLife);
            return;
        }
    }

    
}