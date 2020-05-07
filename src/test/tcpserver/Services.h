

#ifndef __Services_H_INCLUDED__   
#define __Services_H_INCLUDED__   

#include "server.h"

class Services {		
		static void hello(int index);

		static void echo(int index);

		static void process(int readPerCall, char * buff, int index, BOOL & read);
	public:
		static void serve(LPVOID lpParam);
};

#endif 

