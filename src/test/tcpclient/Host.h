

#ifndef __Host_H_INCLUDED__   
#define __Host_H_INCLUDED__   

#undef UNICODE

#define WIN32_LEAN_AND_MEAN

#include <windows.h>
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include <stdio.h>

#include <string>
#include <iostream>

#define DEFAULT_BUFLEN 1024
#define PORT_BUFLEN 5

class Host {	

	int port;

	SOCKET itsSocket;

	FD_SET readSet;				

public:
	Host(SOCKET, int);	

	Host(SOCKET, char *);		

	Host(const char * ip, const char * port);

	Host();	

	~Host();	

	void resetReadSet();

	bool isInReadSet();

	void makeNonBlocking();

	void disconect();

	bool isValid();

	int getService();

	void write(const char *);

	void write(const char *, int);

	int read(char *, int);

	bool canRead();
};


#endif 