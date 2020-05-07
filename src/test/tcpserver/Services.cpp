
#include "services.h"
#include "ConnectionsManager.h"
#include "Globals.h"

void Services::serve(LPVOID lpParam){
	// each thread has its own client
	int index = (int) lpParam;		

	// say welcome
	ConnectionsManager::get(index)
					->write(WELCOME);

	switch (ConnectionsManager::get(index)->getService()) {
	  case HELLO_SERVICE:
		Services::hello(index);
		break;
	  case ECHO_SERVICE:
		Services::echo(index);
		break;
	  default:
		printf("Unknown service requested by client.\n Disconect the client.");
		ConnectionsManager::get(index)->disconect();
		return;
	}

	// anyway disconect the client
	if(ConnectionsManager::get(index) != NULL 
		&& ConnectionsManager::get(index)!= 0 
		&& ConnectionsManager::get(index)->isValid()){

		ConnectionsManager::get(index)->disconect();
		return;
	}	
}


void Services::echo(int index){			
	ConnectionsManager::get(index)->makeNonBlocking();	
	int bytesRead;	
	char readBuf[DEFAULT_BUFLEN];	
			
	BOOL read = true;
	
	// speak cycle
	do {		
		ConnectionsManager::get(index)->resetReadSet();
		//read only when socketObj is ready
		if (ConnectionsManager::get(index)->canRead()) {
			if(ConnectionsManager::get(index)->isInReadSet()){
				bytesRead = ConnectionsManager::get(index)->read(readBuf, DEFAULT_BUFLEN);				
				if (bytesRead > 0) {					
				    if(bytesRead == 1){
						ConnectionsManager::get(index)
							->write("\rTelnet is not supported.\n\rPlease use the normal client.\n\r");
						ConnectionsManager::get(index)
							->write("Good by.\n\r");								
						read = false;						
					} else {					
						readBuf[bytesRead] = '\0';
						process(bytesRead, readBuf, index, read);					
					}
					
				}								
			}
		}		
	} while (read); 
}




void Services::process(int bytesRead, char * cmd, 
					   int index, BOOL & read){		
	
	if(strncmp(cmd, "QUIT", 4) == 0){
		// Send good by								
		ConnectionsManager::get(index)
			->write("Good by.\n");								
		read = false;
	} else {
		// Send echo				
		ConnectionsManager::get(index)
			->write("SERVER:");
		ConnectionsManager::get(index)
			->write(cmd, bytesRead);		
		return;
	}
}


void Services::hello(int index){	
	// Say hello and good by								
	ConnectionsManager::get(index)
		->write("Hello and good by.\n");	
}
