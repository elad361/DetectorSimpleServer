//Author: Elad Shoham 205439649

#include "Server.h"

Server::Server(int port)throw (const char*) {
	struct sockaddr_in server;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		throw "socket failed";
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);
	if (bind(fd, (struct sockaddr*) & server, sizeof(server)) < 0) { throw "bind failure"; }
	if (listen(fd, 3) < 0) { throw "listen failure"; }
}

void Server::start(ClientHandler& ch)throw(const char*){
	struct sockaddr_in client;
	socklen_t clientSize = sizeof(client);
	int clientFD;
	while (clientFD = accept(fd, (struct sockaddr*) & client, &clientSize)) {
		t = new thread([&ch, this, &clientFD]() {ch.handle(clientFD); });
		stop();
	}

}

void Server::stop(){
	t->join();
}

Server::~Server() {
}

string SocketIO::read() throw (const char*){
	std::empty(buffer);
	if ((bytesRead = recv(fd, buffer, READING_BUFF_SIZE, 0)) < 0) {
		throw "Error reading from connection socket";
	}
	return string(buffer);
}
void  SocketIO::write(string text)  throw (const char*) {
	std::empty(buffer);
	text.copy(buffer, text.size() + 1);
	if (send(fd, buffer, text.size(), 0) < 0){
		throw "Error writing data to socket";
	}
}
void  SocketIO::write(float f)  throw (const char*) {
	string s = to_string(f);
	if (send(fd, s.c_str(), s.size(), 0)) {
		throw "Error writing data to socket";
	}
}
void  SocketIO::read(float* f)  throw (const char*) {
	std::empty(buffer);
	if ((bytesRead = recv(fd, buffer, READING_BUFF_SIZE, 0)) < 0) {
		throw "Error reading from connection socket";
	}
	string s(buffer);
	*f = stof(s);
}

void  AnomalyDetectionHandler::handle(int clientID) {
	SocketIO si(clientID);
	string str = si.read();
	CLI cli(&si);
	cli.start();
}

