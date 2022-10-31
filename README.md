
randomChat
=================

The system manages a random chat in which clients connect to a thematic room and come
put in contact with other clients randomly. Once paired clients can
exchange text messages until one of the chat parties closes. Each client is
publicly identified by a nickname, chosen by the user.

### Features

The system through the client offers users the following services:

* Let the user know how many clients are connected in each room.
* Allow the user to wait for a chat in a specific room.
* Once the match has been established, allow the user to exchange messages with the user assigned and to end the conversation at any time.
* Not be assigned to the same counterpart in the same room in two assignments consecutive.
* Provide a maximum length of time for a conversation after which you are assigned to another chat

### Usage client

In chat command:

Write the following command in message box during chat to quit from current chat or request new chat.
```sh
/quit
/newchat
```

### Dependencies

* POSIX Threads
* ncurses

Install dependencies (Ubuntu/Debian)
```sh
sudo apt-get install libncurses5-dev libncursesw5-dev

```

### Compile and run

Compile with a gcc:
```sh
cd randomChat
make

```

#### Server

Run the server `./server/build/Server`

Use `--help` parameter to view all options:

```sh
Usage: ./Server [options]
Options:
-i <address>    Sets the address on which the server is started.
-p <port>       Set the port to use
-t <time>       Set the duration of a chat, <time> is expressed in minutes.
-r <file>       Set the file from which the rooms will be loaded.

```
All [options] fields are not necessary for the program to work,
by default they assume the following values:
address = 127.0.0.1 port = 5200 time = 2
A default room called Main will always be created.


#### Client

Run the client `./client/build/Client`

Use `--help` parameter to view all options:

```sh
Usage: ./Server [options]
Options:
-i <address>    Set the address of the server to connect to.
-p <port>       Set the server port to connect to

```

All fields [options] are not necessary for the program to work,
by default they assume the following values:
address = 127.0.0.1 port = 5200

