curl_lib = -L"C:\Users\Sesa432188\Documents\Cprogramming\curl\lib"
curl_include = -I curl/include

all: main.o rpc_registry.o
	g++ -o main $(curl_lib) rpc_registry.o main.o $(curl_include) -llibcurl
	
main.o: main.cpp Rpc_registry.hpp
	g++ $(curl_lib) -c main.cpp $(curl_include) -llibcurl

rpc_registry.o: Rpc_registry.cpp Rpc_registry.hpp
	g++ -c $(curl_lib) Rpc_registry.cpp $(curl_include) -llibcurl
	

clean :
	rm -f *˜  *. o
mrproper : clean
	rm -f rpc

