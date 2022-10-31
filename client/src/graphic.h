extern pthread_mutex_t output_mutex;

extern WINDOW *chatWin;
extern WINDOW *inputWin;
extern WINDOW *infoWin;

void createWindows(char* opponentNickname);
void createInfoWindow(char* opponentNickname);
void createInputWindow();
void createChatWindow();
void createSepareBox();
void deleteWindows();

void printMessage(WINDOW* win, char* msg, char* nickname, int color);
void printAlert(WINDOW* win, char* msg);