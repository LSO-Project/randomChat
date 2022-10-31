#include <time.h>
#include <pthread.h>
#include <ncurses.h>

#include "graphic.h"

pthread_mutex_t output_mutex = PTHREAD_MUTEX_INITIALIZER;

//Variabili finestre
WINDOW *chatWin;
WINDOW *inputWin;
WINDOW *infoWin;

void createWindows(char* opponentNickname){
    createInfoWindow(opponentNickname);
    createInputWindow();
    createChatWindow();
    createSepareBox();
}

void deleteWindows(){
    delwin(chatWin);
    delwin(inputWin);
    delwin(infoWin);
}

void createInfoWindow(char* opponentNickname){

    int height = 1;
    int width = COLS;
    int starty = 0;
    int startx = 0;

    infoWin = newwin(height, width, starty, startx);

    wbkgd(infoWin, COLOR_PAIR(11));
    wrefresh(infoWin);

    mvwprintw(infoWin, 0, 1, "Chat con: %s",opponentNickname);
}

void createSepareBox(){

    int height = 1;
    int width = COLS;
    int starty = LINES - 2;
    int startx = 0;

    WINDOW* win = newwin(height, width, starty, startx);

    wbkgd(win, COLOR_PAIR(1));

    wattron(win, A_BOLD | COLOR_PAIR(9));
    for(int i=0;i<COLS;i++){
        wprintw(win,"-");
    }
    wattroff(win, A_BOLD | COLOR_PAIR(9));

    wrefresh(win);

}

void createInputWindow(){

    int height = 1;
    int width = COLS;
    int starty = (LINES - 1);
    int startx = 0;

    inputWin = newwin(height, width, starty, startx);

    wbkgd(inputWin, COLOR_PAIR(1));
    wrefresh(inputWin);
}

void createChatWindow(){

    int height = (LINES - 3);  //meno l'altezza delle altre sezioni più il loro padding
    int width = COLS;
    int starty = 1;
    int startx = 0;

    chatWin = newwin(height, width, starty, startx);
    
    wbkgd(chatWin, COLOR_PAIR(1));
    wrefresh(chatWin);
    scrollok(chatWin,TRUE);  //Abilita scroll per scorrimento messaggi in arrivo

}

void printMessage(WINDOW* win, char* msg, char* nickname, int color){

    //Calcola orario invio messaggio
    time_t rawtime;
    struct tm *info;
    time( &rawtime );
    info = localtime( &rawtime );

    pthread_mutex_lock(&output_mutex);

    //Stampa orario
    wattron(win, A_BOLD | COLOR_PAIR(9));
    wprintw(win, "%02d:%02d:%02d", info->tm_hour,info->tm_min,info->tm_sec);
    wprintw(win," <@");
    wattroff(win, A_BOLD | COLOR_PAIR(9));

    wattron(win, A_BOLD | COLOR_PAIR(color));
    wprintw(win,"%s", nickname);
    wattroff(win, A_BOLD | COLOR_PAIR(color));

    wattron(win, A_BOLD | COLOR_PAIR(9));
    wprintw(win,"> ");
    wattroff(win, A_BOLD | COLOR_PAIR(9));

    //Stampa messaggio
    wprintw(win,"%s\n", msg);
    wrefresh(win);

    pthread_mutex_unlock(&output_mutex);

}

void printAlert(WINDOW* win, char* msg){

    pthread_mutex_lock(&output_mutex);

    wattron(win, A_BOLD | COLOR_PAIR(7));
    wprintw(win,msg);
    wrefresh(win);
    wattroff(win, A_BOLD | COLOR_PAIR(7));

    pthread_mutex_unlock(&output_mutex);

}