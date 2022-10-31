#include <signal.h>
#include <stdlib.h>
#include "log.h"

void closeHandler(){
    sysLog("[+] Chiusura server\n");
    exit(1);
}

void bindSignals(){
    signal(SIGINT, closeHandler);
    signal(SIGPIPE, SIG_IGN);
}