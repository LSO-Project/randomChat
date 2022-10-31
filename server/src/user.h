#ifndef USER_H  //include guards
#define USER_H

typedef struct UserStruct {
	char* nickname;

	int socketDescriptor;
	int status;  //Indica lo stato dell'utente: 0 non è collegato a nessuna chat, 1 è collegato a una chat attiva

	struct UserStruct* previusMatch;  //Istanza dell'ultimo match

	//Internal data
	char internalCommand[256];
	char internalData[256];
	  
} User;

User* createUser(void *socketDescriptor);  //Crea nuovo utente

#endif /* USER_H */