#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <iostream>
#include <string>
#include "Rpc_registry.hpp"
using namespace std;

//g++ rpc.cpp -o -I curl/include curl/lib/libcul.a 
//g++ -L"C:\Users\Sesa432188\Documents\Cprogramming\curl\lib" rpc.cpp -o  test -I curl/include -llibcurl

const string Rpc_registry::server_addr = "http://127.0.0.1:5000/rpc/";
 
Rpc_registry::Rpc_registry()
{
	curl_global_init(CURL_GLOBAL_ALL);

	/* init the curl session */ 
	curl_handle = curl_easy_init();
	
}

static size_t
WriteMemoryCallback(void *contents, size_t size, size_t nmemb, void *userp)
{
	size_t realsize = size * nmemb;
	struct MemoryStruct *mem = (struct MemoryStruct *)userp;

	mem->memory =(char *) realloc(mem->memory, mem->size + realsize + 1);
	if(mem->memory == NULL) {
		/* out of memory! */ 
		printf("not enough memory (realloc returned NULL)\n");
		return 0;
	}

	memcpy(&(mem->memory[mem->size]), contents, realsize);
	mem->size += realsize;
	mem->memory[mem->size] = 0;

	return realsize;
}

int Rpc_registry::get_request(const char* method,MemoryStruct* chunk)
{
	CURLcode res;
	
	curl_easy_setopt(curl_handle, CURLOPT_URL, (Rpc_registry::server_addr + method).c_str());

	/* send all data to this function  */ 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

	/* we pass our 'chunk' struct to the callback function */ 
	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)chunk);

	/* some servers don't like requests that are made without a user-agent
	 field, so we provide one */ 
	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	
	/* get it! */ 
	res = curl_easy_perform(curl_handle);
	
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
		curl_easy_strerror(res));
		return -1;
	}
	
	return 1;
}

int Rpc_registry::post_request(const char* method,MemoryStruct* chunk,const string json)
{
	CURLcode res;
	
	curl_easy_setopt(curl_handle, CURLOPT_CUSTOMREQUEST, "POST");
	
	curl_easy_setopt(curl_handle, CURLOPT_POSTFIELDS, json.c_str());
	
	curl_easy_setopt(curl_handle, CURLOPT_URL, (Rpc_registry::server_addr + method).c_str());

	curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, WriteMemoryCallback);

	curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, (void *)chunk);

	curl_easy_setopt(curl_handle, CURLOPT_USERAGENT, "libcurl-agent/1.0");
	
	struct curl_slist *headers = NULL;
	headers = curl_slist_append(headers, "Content-Type: application/json");
	
	curl_easy_setopt(curl_handle, CURLOPT_HTTPHEADER, headers);

	res = curl_easy_perform(curl_handle);
	
	curl_slist_free_all(headers);
	
	if(res != CURLE_OK) {
		fprintf(stderr, "curl_easy_perform() failed: %s\n",
		curl_easy_strerror(res));
		return -1;
	}
	return 1;
}

int Rpc_registry::procede_request(char* method,MemoryStruct* chunk)
{
	return 1;
}

Rpc_registry::~Rpc_registry()
{
	curl_easy_cleanup(curl_handle);
	/* we're done with libcurl, so clean it up */ 
	curl_global_cleanup();
}
