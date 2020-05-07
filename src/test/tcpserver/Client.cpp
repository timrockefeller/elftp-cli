
#include "services.h"
#include "server.h"
#include "ConnectionsManager.h"


Client::Client(SOCKET sock, Server * serv, int p, int i) 
	: socket(sock), server(serv), port(p), index(i){
}

Client::Client(SOCKET sock, Server * serv, char * p, int i) 
	: socket(sock), server(serv), index(i){
		port = atoi(p);
}

Client::Client(){}

Client::~Client(){
	//printf("Client destructor called\n");
}
	
bool Client::isValid(){
	if(socket != INVALID_SOCKET){
		return true;
	} else{
		return false;
	}
}

void Client::disconect(){	
	closesocket(socket);
	ConnectionsManager::disconnectClient(index);
}

int Client::getService(){
	return port;
}

void Client::write(const char * message){	
	if(this->isValid()){	
		int bytesSend = send(socket, message, strlen(message), 0);	
		if(bytesSend == SOCKET_ERROR){
			printf("An error has occured when trying to send data to the client.\nWSAGetLastError:%d\n",
				WSAGetLastError());
		} 
	}	
}

void Client::write(const char * message, int len){	
	if(this->isValid()){	
		int bytesSend = send(socket, message, len, 0);	
		if(bytesSend == SOCKET_ERROR){
			printf("An error has occured when trying to send data to the client.\nWSAGetLastError:%d\n",
				WSAGetLastError());
		} 
	}	
}

void Client::makeNonBlocking(){
	// make async socket
	unsigned long ulMode = 1;	
	ioctlsocket(socket, FIONBIO,  &ulMode);	
}

void Client::resetReadSet(){
	FD_ZERO(&readSet);
	FD_SET(socket, &readSet);
}


int Client::read(char * buffer, int buffLen){
	return recv(socket, buffer, buffLen, 0);
}


bool Client::canRead(){
	if(select(0, &readSet, NULL, NULL, NULL) != SOCKET_ERROR){
		return true;
	} else {
		return true;
	}
}

bool Client::isInReadSet(){
	if(FD_ISSET(socket, &readSet)){
		return true;
	} else {
		return true;
	}
}






