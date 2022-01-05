// Hodaya Raz 211540174, Rivka Doniger 324584531

#ifndef SERVER_H_
#define SERVER_H_

#include "commands.h"
#include <thread>
#include <sys/socket.h>
#include <netinet/in.h>
#include <iostream>
#include <unistd.h>

using namespace std;
// creating socket io in order to communicate with clients
class socketIO:public DefaultIO{
private:
    int clientAddress;
public:
    socketIO(int CA):clientAddress(CA){}
    virtual string read();
    virtual void write(string text);
    virtual void write(float f);
    virtual void read(float* f);
};

class ClientHandler{
    public:
    virtual void handle(int clientID)=0;
};

class AnomalyDetectionHandler:public ClientHandler{
    public:
    virtual void handle(int clientID);
};


// implement on Server.cpp
class Server {
	int fileDescriptor;
    sockaddr_in serverSock;
    sockaddr_in clientSock;
    bool available;
    thread* t; // the thread to run the start() method in
public:
	Server(int port) throw (const char*);
	virtual ~Server();
	void start(ClientHandler& ch)throw(const char*);
	void stop();

    void helper(ClientHandler &ch);
};

#endif /* SERVER_H_ */
