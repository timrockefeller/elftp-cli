
#include "host.h" 
#include "Functions.h" 

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
// #pragma comment (lib, "Mswsock.lib")

WSADATA wsaData;

int initWinsock(){
	// Initialize Winsock
	int iResult = WSAStartup(MAKEWORD(2,2), &wsaData);
	if (iResult != 0) {
		printf("WSAStartup failed with error: %d\n", iResult);
		return 1;
	} else {
		// printf("WSAStartup ok!\n");
	} 
	return iResult;
}


int main(int argc, char * argv[]) 
{					
	

	if (initWinsock() != 0) {
		return 1;
	}
		
	if(argc != 3){
		printf("Specify the ip address and port.\n");
		return 1;
	}	

	Host * host = new Host(argv[1], argv[2]);		

	//Host * host = new Host("91.203.147.237", "25");
	//Host * host = new Host("127.0.0.1", "2811");

	// pass host to network thread
	CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)netLoop, (LPVOID)host, 0, NULL); 

	// run console loop 
	consoleLoop(host);
	
	return 0;
}