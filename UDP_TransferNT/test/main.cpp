#include <iostream>
#include "UDP_TransferNT.h"

#ifdef NT_UDP_PLATFORM_WINDOWS
#include <Windows.h>
#elif defined(NT_UDP_PLATFORM_WINDOWS)
#include <system>
#endif

#include <thread>

using namespace UDP_TransferNT;

int cycleCount = 20;

void server() {
	Network s_network(Network::Type::SERVER, Network::ConnectionType::IP_SPECIFIC);
	s_network.init();

	DataPacket dpRecv;
	for (int i = 0; i < cycleCount; i++) {
		dpRecv = s_network.dpRecv(dpRecv);
		std::cout << "DP Recv: " << dpRecv.getDecimals(0) << std::endl;
		Sleep(1000);
	}
}

void client() {
	Network c_network(Network::Type::CLIENT, Network::ConnectionType::IP_SPECIFIC);
	c_network.init();

	DataPacket dpSend;

	for (int i = 0; i < cycleCount; i++) {
		dpSend.setDecimals(0, 0.5);
		c_network.dpSend(dpSend);
		// c_network.raw_send("hello from client");
		Sleep(1000);
	}
}

int main() {
	std::cout << "Test Run..." << std::endl;

	// server();
	// client();

	std::thread server_t(server);
	std::thread client_t(client);

	server_t.join();
	client_t.join();

	system("pause");

	return 0;
}