Author: Elad Shoham 205439649

#include <thread>
#include <iostream>
//#include <winsock2.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <pthread.h>
#include <thread>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "commands.h"
//#include <winsock.h>
#include "CLI.h"

#ifndef SERVER_H_
#define SERVER_H_

#define READING_BUFF_SIZE 3000


using namespace std;

class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};


class AnomalyDetectionHandler:public ClientHandler{
	public:
		virtual void handle(int clientID);
};

class SocketIO : public DefaultIO {
	int fd;
	int bytesRead;
	char buffer[READING_BUFF_SIZE]{};
public:
	SocketIO(int fileDescription) : DefaultIO(), fd(fileDescription), bytesRead(0){}
	
	virtual ~SocketIO() {}

	virtual string read();

	virtual void write(string text);

	virtual void write(float f);

	virtual void read(float* f);

};
// implement on Server.cpp
class Server {
	thread* t; 
	int fd;

public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();
};

#endif /* SERVER_H_ */
