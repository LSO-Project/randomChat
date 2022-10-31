//Thread controller
void* timeController(void* val);
void* inputController(void* val);
void* communicationController(void* val);

int chatController();

void sendExitRes(int val);
int chat();

extern char* myNickname;
extern char* opponentNickname;
extern int chatLife;