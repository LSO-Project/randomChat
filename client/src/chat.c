#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <string.h>
#include <time.h>
#include <ncurses.h>

#include "chat.h"
#include "connection.h"
#include "graphic.h"

int active;  //Variabile che indica lo stato della chat (1 attiva, 0 terminata)(per sincronizzare thread)
char internalCmd[256];  //Comando interno per comunicazione tra i vari thread e il chatController

//Serve per evitare errori di timing tra client e server
void sendExitRes(int val){

    switch (val){
   	    case 0:
   	        sendMessage("res:userLeave");  //Comunica al server(proprio thread) l'uscita
   	    	break;
 	    case 1:
   	    	sendMessage("res:opponentLeave");  //Comunica al server(proprio thread) che l'opponent ha abbandonato
   	    	break;
 	    case 2:
   	    	sendMessage("res:userRequestNewChat");  //Comunica al server(proprio thread) l'uscita
   	    	break;
	    case 3:
	     	sendMessage("res:timeExpired");  //Comunica al server(proprio thread) che il tempo a disposizione è terminato
	     	break;
	}
}

int chatController(){
   
    while(1){

        //L'utente fa quit
        if(strcmp(internalCmd,"intCmd:userLeave")==0){   
            return 0;
        }
        
        //L'opponent ha abbandonato la chat
        if(strcmp(internalCmd,"intCmd:opponentLeave")==0){   
            printAlert(chatWin,"Chat terminata, premere invio per continuare...\n");         
            return 1;
        }

        //L'utente fa newchat
        if(strcmp(internalCmd,"intCmd:userRequestNewChat")==0){   
            return 2;
        }

        //Il tempo è scaduto
        if(strcmp(internalCmd,"intCmd:timeExpired")==0){   
            printAlert(chatWin,"Tempo terminato, premere invio per continuare...\n");
            return 3;
        }

        //Si è verificato un'errore di connessione al server
        if(strcmp(internalCmd,"intCmd:connectionError")==0){   
            printAlert(chatWin,"Errore di connessione\n");
            return -1;
        }

        //sleep(1);
        napms(500);
    }

    return 0;

}

void* communicationController(void* val){

    while(checkConnection() && (active == 1)){
        
        if(checkMessage()==1){
            char* msg = reciveMessage();

            //L'opponent ha abbandonato la chat
            if(strcmp(msg,"cmd:userLoggedOut")==0){   
                strcpy(internalCmd,"intCmd:opponentLeave");
                active = 0;                
                return NULL;
            }

            printMessage(chatWin,msg,opponentNickname,10);  //Stampa messaggio
            free(msg);
        }

        //msleep(200);
        napms(200);
    }

    //Verifica se si è presentato un'errore di connessione
    if(active == 1){
        strcpy(internalCmd,"intCmd:connectionError");
        active = 0;
        return NULL;
    }
    return NULL;
}

void* inputController(void* val){

    while(active==1){

        pthread_mutex_lock(&output_mutex);
        wbkgd(inputWin, COLOR_PAIR(1));
        mvwprintw(inputWin, 0, 0, "#%s> ", myNickname);
        wrefresh(inputWin);
        pthread_mutex_unlock(&output_mutex);
    
        char buffer[256];
        memset(buffer,0,sizeof(buffer));
        wgetstr(inputWin,buffer);  //Legge input 

        //L'utente digita il comando quit
        if(strcmp(buffer,"/quit")==0){
            strcpy(internalCmd,"intCmd:userLeave");
            active = 0;
            return NULL;
        }

        //L'utente digita il comando newchat
        if(strcmp(buffer,"/newchat")==0){
            strcpy(internalCmd,"intCmd:userRequestNewChat");
            active = 0;
            return NULL;
        }

        if(active == 1){
            printMessage(chatWin,buffer,myNickname,8);  //Stampa messaggio
            sendMessage(buffer);  //Invia messaggio a server
        }

        pthread_mutex_lock(&output_mutex);
        wclear(inputWin);
        wrefresh(inputWin);
        pthread_mutex_unlock(&output_mutex);

    }
    return NULL;
}

void* timeController(void* val){

    //Variabili per controllo tempo
    int min = chatLife;
    int sec = min*60;
    time_t start = time(NULL) + sec;
    
    while(active==1){

        //Controllo tempo rimanente
        if(start > time(NULL)){
            time_t left = (start - time(NULL));
            struct tm* ptm = localtime(&left);
            pthread_mutex_lock(&output_mutex);
            mvwprintw(infoWin, 0, (COLS - 25), "Tempo rimanente: %02d:%02d",ptm->tm_min, ptm->tm_sec);  //Fa print orario
            wrefresh(infoWin);  //Aggiorna finestra
            pthread_mutex_unlock(&output_mutex);
        }else{
            strcpy(internalCmd,"intCmd:timeExpired");
            active = 0;
            return NULL;
        }
        sleep(1);
    }
    return NULL;
}

int chat()
{
    clear();
    
    active = 1;

    start_color();

    assume_default_colors(-1,-1);

    //Settaggio colori
    init_pair(1, -1, -1);
    init_pair(2, COLOR_WHITE, COLOR_BLUE);
    init_pair(5, COLOR_BLACK, COLOR_WHITE);
    init_pair(7, COLOR_WHITE, COLOR_RED);
    init_pair(8, COLOR_GREEN, -1);
    init_pair(9, 8, -1);  //Grigio e colore term
    init_pair(10, COLOR_MAGENTA, -1);
    init_pair(11, COLOR_WHITE, COLOR_GREEN);
    init_pair(12, COLOR_WHITE, COLOR_RED);

    //bkgd(COLOR_PAIR(12));
    refresh();

    createWindows(opponentNickname);
    
    memset(internalCmd,0,sizeof(internalCmd));  //Inizializza variabile interna

    pthread_t tidCommunicationController;
    pthread_create(&tidCommunicationController,NULL, communicationController, NULL);  //Avvia thread per controllo comunicazione

    pthread_t tidInputController;
    pthread_create(&tidInputController,NULL, inputController, NULL);  //Avvia thread per finesta di input

    pthread_t tidTimeController;
    pthread_create(&tidTimeController,NULL, timeController, NULL);  //Avvia thread per controllo tempo rimanente

    int res = chatController();  //Avvia chatController
    pthread_join(tidTimeController, NULL);
    pthread_join(tidCommunicationController, NULL); 
    pthread_join(tidInputController, NULL); 

    sendExitRes(res);  //Comunica al server il comando di uscita
    
    deleteWindows();

    clear();
    flushinp();
    
    return res;
}