

#ifndef __Client_H_INCLUDED__   
#define __Client_H_INCLUDED__   

#include "server.h"

class Client {	

	int port;

	SOCKET socket;

	Server * server;

	FD_SET readSet;			

	int canReed;

	int index;

public:
	Client(SOCKET, Server *, int, int);	

	Client(SOCKET, Server *, char *, int);		

	Client();	

	~Client();	

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

