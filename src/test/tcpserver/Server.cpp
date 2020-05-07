
#include "server.h"
#include "services.h"
#include "ConnectionsManager.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

class Server {	
	WSADATA wsaData;	
	char port[PORT_BUFLEN];

	SOCKET serverSocket;    

	struct addrinfo * result;
    struct addrinfo hints;	

	int initWinsock(){
		// Initialize Winsock
		int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
		if (iResult != 0) {
			printf("WSAStartup failed with error: %d\n", iResult);
			return 1;
		} else {
			printf("WSAStartup ok!\n");
		} 
		return iResult;
	}

	void configure(){
		ZeroMemory(&hints, sizeof(hints));
		hints.ai_family = AF_INET;
		hints.ai_socktype = SOCK_STREAM;
		hints.ai_protocol = IPPROTO_TCP;
		hints.ai_flags = AI_PASSIVE;
	}

	int resolveServer(){
		int iResult = getaddrinfo(NULL, port, &hints, &result);
		if ( iResult != 0 ) {
			printf("getaddrinfo failed with error: %d\n", iResult);			
			return 1;
		} else {
			printf("Resolving server ok!\n");
		} 
		return iResult;	
	}

	int createServerSocket(){
		serverSocket = socket(result->ai_family, result->ai_socktype, result->ai_protocol);
		if (serverSocket == INVALID_SOCKET) {
			printf("creating listen socket failed with error: %ld\n", WSAGetLastError());
			return 1;
		}
		return 0;
	}

	int bindSocket(){
		int iResult = bind(serverSocket, result->ai_addr, (int)result->ai_addrlen);
		if (iResult == SOCKET_ERROR) {
			printf("bind failed with error: %d\n", WSAGetLastError());
			return 1;
		}
		return 0;
	}

public:
	Server(const char * p) : serverSocket(INVALID_SOCKET), result(NULL){					
		// copy port value 		
		strcpy_s(port, sizeof(port), p);

		// init WinSock
		int iRes = initWinsock();
		if(iRes != 0){ return; }

		// prepare info structs
		configure();

		// setting up our server
		iRes = resolveServer();	
		if(iRes != 0){ return; }

		// create listen socket object 
		// based on retrieved info
		iRes = createServerSocket();
		if(iRes != 0){ return; }

		// associate socket with local address
		iRes = bindSocket();
		if(iRes != 0){ return; }			
	}

	~Server(){
		if(serverSocket != INVALID_SOCKET){
			closesocket(serverSocket);
		}
		if(result != NULL){
			freeaddrinfo(result);
		}		
		WSACleanup();	
	}	
	
	void run(){
		// enable non-blocking mode on serverSocket
		unsigned long ulMode = 1;		
		ioctlsocket(serverSocket, FIONBIO, &ulMode);

		// place socket in state in which 
		// it is listenting for connections
		listen(serverSocket, SOMAXCONN);

		// set of sockets which 
		// are ready to I/O opearations
		FD_SET socketsSet;
		
		// result of calling to select function
		int selectRes;

		printf("Server is running.\nWaiting for connections...\n");

		// listen cycle
		while (1){
			// clean the set
			FD_ZERO(&socketsSet);

			// put socket into a set
			FD_SET(serverSocket, &socketsSet);

			// try to select sockets which are ready for I/O
			if ((selectRes = select(0, &socketsSet, NULL, NULL, NULL)) == SOCKET_ERROR) {				
				continue;
			}

			// check if we have our serverSocket in socketsSet
			if(FD_ISSET(serverSocket, &socketsSet)){
				// we have a new connection to server
				SOCKET clientSocket = accept(serverSocket, NULL, NULL);
				if (clientSocket == INVALID_SOCKET) {
					printf("accept failed with error: %d\n", WSAGetLastError());
					continue;
				}
								
				if(ConnectionsManager::suspend == false){
					// create separate thread to work with client									
					int index = ConnectionsManager::addConnection(clientSocket, this, port);			
					if(index >= 0){						
						CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)Services::serve, (LPVOID)index, 0, 0); 
					}
														
				} else {
					printf("Server suspended.\nPlease try again in a while.");
					closesocket(clientSocket);
				}		
				
				
			}
			
		}
	}
};


int main(int argc, char * argv[]) 
{				
	/*
	if(argc != 2){
		printf("Specify the service as first argument.\n");
		return 1;
	}	
	Server server(argv[1]);	
	*/
	Server server("2811");	
	server.run();
	ConnectionsManager::disconnectAllClients();
	return 0;
}
