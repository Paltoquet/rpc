#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <iostream>
#include "Rpc_registry.hpp"

using namespace std;


//C:\MinGW\bin\mingw32-make 
int main(void)
{
	Rpc_registry rpc;
	struct MemoryStruct chunk;
	chunk.memory =(char *) malloc(1);
	chunk.size = 0; 
	
	if(rpc.get_request("display",&chunk) > 0){
		printf("%lu bytes retrieved\n", (long)chunk.size);
		printf("value %s",chunk.memory);
	}
	
	chunk.memory =(char *) malloc(1);
	chunk.size = 0; 

	if(rpc.post_request("method",&chunk,"{\"method\":\"hello\",\"params\":[\"bob\",\"1\",\"Naboo\"],\"id\":2}") > 0){
		printf("%lu bytes retrieved\n", (long)chunk.size);
		printf("value %s",chunk.memory);
	}
	
	return 0;
}