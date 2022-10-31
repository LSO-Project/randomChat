OBJECTS = src/main.o src/connection.o src/menu.o src/room.o src/lobby.o src/chat.o src/graphic.o
CFLAGS=-g -Wall
LIBS = -lpthread -lncurses
CC = gcc
PROGRAM_NAME = build/Client

$(PROGRAM_NAME):$(OBJECTS)
		$(CC) $(CFLAGS) -o $(PROGRAM_NAME) $(OBJECTS) $(LIBS)
		@echo " "
		@echo "Compilation completed"
		@echo " "

clean:
		rm -f src/*.o
		rm $(PROGRAM_NAME)