
#ifndef __ConnectionsManager_H_INCLUDED__   
#define __ConnectionsManager_H_INCLUDED__   

#include "server.h"
#include "services.h"
#include "client.h"

#define MAX_CONNECTIONS 3

class ConnectionsManager {
	
public:	
	static int disconNum;

	static int disconIndexes[MAX_CONNECTIONS];

	static bool suspend;

	static Client * connections[MAX_CONNECTIONS];

	static int connIndex;	

	static void disconnectClient(int index);

	static void disconnectAllClients();

	static bool canConnect();	

	static int addConnection(SOCKET sock, Server * serv, char * p);	

	static Client * get(int index);

};

#endif 

