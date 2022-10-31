OBJECTS = src/main.o src/server.o src/user.o src/room.o src/chat.o src/log.o src/signals.o src/connection.o src/roomList.o src/userList.o src/chatList.o
CFLAGS=-g -Wall
LIBS = -lpthread
CC = gcc
PROGRAM_NAME = build/Server

$(PROGRAM_NAME):$(OBJECTS)
		$(CC) $(CFLAGS) -o $(PROGRAM_NAME) $(OBJECTS) $(LIBS)
		@echo " "
		@echo "Compilation completed"
		@echo " "

clean:
		rm -f src/*.o
		rm $(PROGRAM_NAME)