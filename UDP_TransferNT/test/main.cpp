#include <iostream>
#include "UDP_TransferNT.h"

#ifdef NT_UDP_PLATFORM_WINDOWS
#include <Windows.h>
#elif defined(NT_UDP_PLATFORM_WINDOWS)
#include <system>
#endif

#include <thread>

using namespace UDP_TransferNT;

void server() {
	Network s_network(Network::Type::SERVER, Network::ConnectionType::ANY);
	s_network.init();

	DataPacket dpRecv;
	while (true) {
		dpRecv = s_network.dpRecv(dpRecv);
		std::cout << "DP Recv: " << dpRecv.getDecimals(0) << std::endl;
		Sleep(1000);
	}
}

void client() {
	Network c_network(Network::Type::CLIENT, Network::ConnectionType::ANY);
	c_network.init();

	DataPacket dpSend;

	while (true) {
		dpSend.setDecimals(0, 0.5);
		c_network.dpSend(dpSend);
		Sleep(1000);
	}
}

int main() {
	std::cout << "Test Run..." << std::endl;


	std::thread server_t(server);
	std::thread client_t(client);

	server_t.join();
	client_t.join();


	return 0;
}