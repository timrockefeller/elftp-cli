
#include "Functions.h"

void netLoop(LPVOID lpParam){
	Host * host = (Host *)lpParam;
	host->makeNonBlocking();	
	int bytesRead;	
	char readBuf[DEFAULT_BUFLEN];		
	
	BOOL read = true;
	
	// read write cycle
	do {		
		host->resetReadSet();
		//read only when socket is ready
		if (host->canRead()) {
			if(host->isInReadSet()){
				bytesRead = host->read(readBuf, DEFAULT_BUFLEN);				
				if (bytesRead > 0) {					
					readBuf[bytesRead] = '\0';
					printf(readBuf);					
				}								
			}
		}		
	} while (read); 
	host = 0;
}