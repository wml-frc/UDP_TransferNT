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
	client.getSocket().setIP("192.168.178.125");
	client.getSocket().setPort(5801);
	client.init();
	#endif

	#ifdef SERVER_
	server.init();
	#endif

	#ifdef CLIENT_
	client.registerSend(dpSend);
	#endif

	#ifdef SERVER_
	server.registerRecv(dpRecv);
	#endif

	float value = 0;

	while(true) {
		system("clear");
		std::cout << "Registered client sender, and server receiver" << std::endl;

		#ifdef SERVER_
		std::cout << "Booleans index 1: " << dpRecv.getBooleans(1) << std::endl;
		std::cout << "Characters index 10: " << dpRecv.getCharacters(10) << std::endl;
		std::cout << "Decimals index 0: " << dpRecv.getDecimals(0) << std::endl;
		std::cout << "Integers index 2: " << dpRecv.getIntegers(2) << std::endl;
		#endif

		value += 0.001;
		dpSend.setDecimals(0, value);
		sleep(1);
	}


	#else
	#endif
	return 0;
}