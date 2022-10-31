#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>
#include <pthread.h>
#include <time.h>
#include "log.h"

pthread_mutex_t log_mutex;
FILE *log_file;

void sysLog(const char* format, ...){

    char* value = (char*)malloc(sizeof(char)*512);
    memset(value,0,512);
    va_list ap;
    va_start(ap, format);
    vsnprintf(value, 512 ,format,ap);
    va_end(ap);

    time_t rawtime;
    struct tm * timeinfo;

    time (&rawtime);
    timeinfo = localtime (&rawtime);

    pthread_mutex_lock(&log_mutex);

    printf("%s\n", value);

    openLog();
    fprintf(log_file, "[%02d/%02d/%d - %02d:%02d:%02d] %s\n",  timeinfo->tm_mday, timeinfo->tm_mon+1, timeinfo->tm_year +1900,timeinfo->tm_hour,timeinfo->tm_min,timeinfo->tm_sec,value);
    closeLog();

    pthread_mutex_unlock(&log_mutex);

}

void openLog(){
    log_file = fopen("log.txt", "a+");
    if(!log_file)
        printf("Errore apertura file di log\n");
}

void closeLog(){
    fclose(log_file);
}