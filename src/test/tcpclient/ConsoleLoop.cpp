
#include "Functions.h"

void consoleLoop(Host * host) 
{					
		
	// listen console
	std::string line;	
	char buff[DEFAULT_BUFLEN];
	while (1)
	{
		if(std::getline(std::cin, line)){	
			memcpy(buff, line.c_str(), line.size());
			buff[line.size()] = '\n';
			buff[line.size()+1] = '\0';
			host->write(buff);	

			if(strncmp(line.c_str(), "QUIT", 4) == 0){
				break;
			}
		}
		
	}
	
}