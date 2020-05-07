
#include "host.h"

Host::Host(SOCKET sock, int p) 
	: itsSocket(sock), port(p){
}

Host::Host(SOCKET sock, char * p) 
	: itsSocket(sock){
		port = atoi(p);
}

Host::Host(const char * ip, const char * port){
	struct addrinfo hints;
	struct addrinfo * result = NULL;
	struct addrinfo * ptr = NULL;
	                
	ZeroMemory( &hints, sizeof(hints) );
	hints.ai_family = AF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = IPPROTO_TCP;

	int iResult;

	// Resolve the server address and port
	iResult = getaddrinfo(ip, port, &hints, &result);
	if (iResult != 0) {
		//resolve failed
		printf("getaddrinfo failed with error: %d\n", iResult);	
		return;
	}

	SOCKET socketObj = INVALID_SOCKET;

	// Attempt to connect to the first address returned by
	// the call to getaddrinfo
	ptr = result;

	// Create a SOCKET for connecting to server
	socketObj = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);

	if (socketObj == INVALID_SOCKET) {
		// try to use another addrinfo in list
		while(ptr = ptr->ai_next){
			// Create a SOCKET for connecting to server
			socketObj = socket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol);
			if (socketObj == INVALID_SOCKET) {
				continue;
			} else {
				break;
			}
		}		
	}

	if (socketObj == INVALID_SOCKET) {
		freeaddrinfo(result);		
		printf("Cannot instantiate an object of type SOCKET: %d\n", WSAGetLastError());	
		return;
	}

	// Connect unconnected socketObj to host.		
	if ((connect(socketObj, ptr->ai_addr, (int)ptr->ai_addrlen)) == SOCKET_ERROR) {
		closesocket(socketObj);	
		printf("Cannot connect SOCKET to remote host: %d\n", WSAGetLastError());	
		return;
	}

	// if we here the socketObj is good
	itsSocket = socketObj;
}

Host::Host(){}

Host::~Host(){
	//printf("Host destructor called\n");
}
	
bool Host::isValid(){
	if(itsSocket != INVALID_SOCKET){
		return true;
	} else{
		return false;
	}
}

void Host::disconect(){	
	closesocket(itsSocket);	
}

int Host::getService(){
	return port;
}

void Host::write(const char * message){	
	if(this->isValid()){	
		int bytesSend = send(itsSocket, message, strlen(message), 0);	
		if(bytesSend == SOCKET_ERROR){
			printf("An error has occured when trying to send data to the client.\nWSAGetLastError:%d\n",
				WSAGetLastError());
		} 
	}	
}

void Host::write(const char * message, int len){	
	if(this->isValid()){	
		int bytesSend = send(itsSocket, message, len, 0);	
		if(bytesSend == SOCKET_ERROR){
			printf("An error has occured when trying to send data to the client.\nWSAGetLastError:%d\n",
				WSAGetLastError());
		} 
	}	
}

void Host::makeNonBlocking(){
	// make async socket
	unsigned long ulMode = 1;	
	ioctlsocket(itsSocket, FIONBIO,  &ulMode);	
}

void Host::resetReadSet(){
	FD_ZERO(&readSet);
	FD_SET(itsSocket, &readSet);
}


int Host::read(char * buffer, int buffLen){
	return recv(itsSocket, buffer, buffLen, 0);
}


bool Host::canRead(){
	if(select(0, &readSet, NULL, NULL, NULL) != SOCKET_ERROR){
		return true;
	} else {
		return true;
	}
}

bool Host::isInReadSet(){
	if(FD_ISSET(itsSocket, &readSet)){
		return true;
	} else {
		return true;
	}
}