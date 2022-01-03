// Hodaya Raz 211540174, Rivka Doniger 324584531
#include "Server.h"
#include "CLI.h"


Server::Server(int port)throw (const char*) {
    available = true;
    // type v4 address
    serverSock.sin_family = AF_INET;
    // so that we can bind any ip address to the server
    serverSock.sin_addr.s_addr = INADDR_ANY;
    // so that the port number is translated from host byte order to network byte order
    serverSock.sin_port = htons(port);
    // open a socket to get file descriptor
    fileDescriptor = socket(AF_INET, SOCK_STREAM, 0);
    // if opening the socket fails
    if (fileDescriptor < 0) {
        throw "socket failed to open";
    }
    int bindReturn = bind(fileDescriptor, (struct sockaddr*)&serverSock, sizeof(serverSock));
    if (bindReturn < 0) {
        throw "binding failed";
    }
    int listenReturn = listen(fileDescriptor, 5);
    if (listenReturn < 0) {
        throw "listening failed";
    }
}

void alarmSignal(int numAlarm){
    cout<<"time-out"<<endl;
    exit(numAlarm);
}

void Server::start(ClientHandler& ch)throw(const char*){
    t=new thread([&ch,this](){
        signal(SIGALRM, alarmSignal);
        while(available){
            socklen_t cSocketSize=sizeof(clientSock);
            alarm(1);
            int currentAcceptedClientID = accept(fileDescriptor, (struct sockaddr*)&clientSock, &cSocketSize);
            if(currentAcceptedClientID > 0){
                ch.handle(currentAcceptedClientID);
                close(currentAcceptedClientID);
            }
            alarm(0);

        }
        close(fileDescriptor);
    });
}

void Server::stop(){
	available = false;
    t->join(); // do not delete this!
}

Server::~Server() {
}

string socketIO::read() {
    // to read each char from client
    char temp = '0';
    // for holding the data sent from client
    string data = "";
    while (temp != '\n') {
        recv(clientAddress, &temp, sizeof(temp), 0);
        data += temp;
    }
    return data;
}

void socketIO::write(string text) {
    // change to fit parameter for send
    const char* sentData = text.c_str();
    send(clientAddress, sentData, strlen(sentData), 0);
}

void socketIO::write(float f) {
    ostringstream stringStream;
    // put float into string stream
    stringStream << f;
    // convert what is in the string stream into string str
    string str(stringStream.str());
    write(str);
}

void socketIO::read(float *f) {
    recv(clientAddress, f, sizeof(f), 0);
}

void AnomalyDetectionHandler::handle(int clientID) {
    // create socketIO
    socketIO socketIo(clientID);
    CLI commandLine(&socketIo);
    commandLine.start();
}
