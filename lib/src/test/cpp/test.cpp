/*
 * This C++ source file was generated by the Gradle 'init' task.
 */
#define CLIENT_
#define SERVER_

#include "UDP_TransferNT.h"
#include <cassert>

int main() {
	// assert(greeter.greeting().compare("Hello, World!") == 0);

	#ifdef __linux__
	std::cout << "Test Start" << std::endl;
	UDP_TransferNT::DataPacket dpSend, dpRecv;
	
	dpSend.setBooleans(1, true);
	dpSend.setCharacters(10, 't');
	dpSend.setDecimals(0, 0.111);
	dpSend.setIntegers(2, 12);

	UDP_TransferNT::Client client(true);
	UDP_TransferNT::Server server(true);

	#ifdef CLIENT_
	client.getSocket().setIP("127.0.0.1");
	client.getSocket().setPort(5801);
	client.init();
	#endif

	#ifdef SERVER_
	server.init();
	#endif

	#ifdef CLIENT_
	// client.registerSend(dpSend);
	#endif

	#ifdef SERVER_
	// server.registerRecv(dpRecv);
	#endif

	float value = 0;

	

	#else
	#endif
	return 0;
}