#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <map>
#include <string>
#include <iostream>
using namespace std;

struct MemoryStruct {
  char *memory;
  size_t size;
};


class Rpc_registry
{
private:
	CURL *curl_handle;
	std::map<char*,int*> registry;
public:
	static const std::string server_addr;

	Rpc_registry();
	int get_request(const char* method,MemoryStruct* chunk);
	int post_request(const char* method,MemoryStruct* chunk,const string json);
	int procede_request(char* method,MemoryStruct* chunk);
	virtual ~Rpc_registry();
};