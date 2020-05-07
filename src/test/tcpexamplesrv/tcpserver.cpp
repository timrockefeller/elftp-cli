
#include <stdio.h>
#include <winsock2.h>
#pragma comment(lib, "ws2_32.lib")
void main(int argc, char *argv[]) {

    //initiate Winsock
    WSADATA wsaData;
    int iResult = WSAStartup( MAKEWORD(2,2), &wsaData );
    if ( iResult != NO_ERROR )
        printf("Error at WSAStartup()\n");

    //create server's socket
    SOCKET welcomeSocket;
    welcomeSocket = socket( AF_INET, SOCK_STREAM, IPPROTO_TCP );

    if ( welcomeSocket == INVALID_SOCKET ) {
        printf( "Error at socket(): %ld\n", WSAGetLastError() );
        WSACleanup();
        return;
    }

    //bind server socket with the port and ip address
    sockaddr_in service;

    service.sin_family = AF_INET;
    service.sin_addr.s_addr = INADDR_ANY;
    service.sin_port = htons(atoi(argv[1]));

    if ( bind( welcomeSocket, (SOCKADDR*) &service, sizeof(service) ) == SOCKET_ERROR ) {
        printf( "bind() failed.\n" );
        closesocket(welcomeSocket);
        return;
    }
    
    //listening socket binded with port
    if ( listen( welcomeSocket, 10 ) == SOCKET_ERROR )
        printf( "Error listening on socket.\n");

    while (1) {
        SOCKET connectionSocket=SOCKET_ERROR;
        int i=0, n=0;
        char      clientSentence[200] = {0};
        char capitalizedSentence[200] = {0};

        while ( connectionSocket == SOCKET_ERROR ) {
            connectionSocket = accept( welcomeSocket, NULL, NULL );
        }

        n=recv(connectionSocket, clientSentence, sizeof(clientSentence), 0); 
 
        if(n>199) 
        {
            clientSentence[199]=0;
            closesocket(connectionSocket);
            continue;
        }
           
        for(i=0;i<strlen(clientSentence);i++)
            capitalizedSentence[i]=
                (clientSentence[i]>='a' && clientSentence[i]<='z')
                    ?clientSentence[i]-32:clientSentence[i];

        capitalizedSentence[i]='\0';

        n=send(connectionSocket, capitalizedSentence, strlen(capitalizedSentence), 0); 
        closesocket(connectionSocket);
 
     }
    
    return;
}

