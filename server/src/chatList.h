#include "chat.h"

//Struttura lista
struct ChatNode {
	Chat* chat;
	struct ChatNode* next;
};

typedef struct ChatNode* ChatList;

ChatList initChatNode(Chat* chat);
ChatList appendChatNode(ChatList L, Chat* chat);
ChatList removeChatNode(ChatList L, Chat* target);

Chat* getChatById(ChatList L, char* idChat);
Chat* checkUserInChatList(ChatList L, User* user);  //Verifica se user è presente in qualche chat in tal caso ritorna la chat, altrimenti ritorna null
