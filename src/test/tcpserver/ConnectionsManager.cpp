
	#include "ConnectionsManager.h"	

	Client * ConnectionsManager::connections[MAX_CONNECTIONS];

	int ConnectionsManager::disconIndexes[MAX_CONNECTIONS];

	int ConnectionsManager::connIndex = 0;	

	bool ConnectionsManager::suspend = false;

	int ConnectionsManager::disconNum = 0;	

	Client * ConnectionsManager::get(int index){
		if(index >= 0 && index < MAX_CONNECTIONS){
			return connections[index];
		}
		return 0;
	}

	bool ConnectionsManager::canConnect(){		
		return ((connIndex - disconNum) >= MAX_CONNECTIONS) ? false : true;
	}	

	void ConnectionsManager::disconnectClient(int index){					
		if(index >= 0 && index < MAX_CONNECTIONS){
			if(connections[index] != 0){				
				delete connections[index];
				connections[index] = 0;

				// add index to disconnected				
				disconIndexes[++disconNum] = index;				
				printf("Client disconected.\n");
			}
		}		
	}

	int ConnectionsManager::addConnection(SOCKET sock, Server * serv, char * p){
		if(canConnect() == false){
			printf("Connections limit has been reached.\nDiscard connection.\n");						
		} else {		
			int ret;			
			if(disconNum > 0){
				// use free index				
				int discIndex = ret = disconIndexes[disconNum--];			
				connections[discIndex] = new Client(sock, serv, p, discIndex);														
			} else {
				// use new index
				connections[connIndex] = new Client(sock, serv, p, connIndex);	
				ret = connIndex++;				
			}			
			printf("New connection.\n");
			return ret;
		}
		return -1;
	}
	

	void ConnectionsManager::disconnectAllClients(){
		for(int i = 0; i < MAX_CONNECTIONS; i++){
			disconnectClient(i);
		}	
	}
