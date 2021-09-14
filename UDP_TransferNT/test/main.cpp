#include <iostream>
#include <fstream>
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
	// s_network.getSocket().setRecvTimeout(1);
	s_network.init();

	DataPacket dpRecv;
	// while (true) {
	for (int i = 0; i < cycleCount; i++) {

		// char *buffer;
		// memset(buffer, '\0', DEFAULT_BUFFER_SIZE);
		
		char *buffer;
		buffer = (char *)malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
		// buffer = "Hello from server";
		// s_network.raw_recv(buffer);
		// s_network.raw_send(buffer);
		s_network.raw_recv(buffer);
		std::cout << "From Buffer: " << buffer << std::endl;
		free(buffer);
		Sleep(1000);
		// dpRecv = s_network.dpRecv(dpRecv);
		// std::cout << "DP Recv: " << dpRecv.getDecimals(0) << std::endl;
	}
}

void client() {
	Network c_network(Network::Type::CLIENT, Network::ConnectionType::IP_SPECIFIC);
	c_network.init();

	DataPacket dpSend;

	// while (true) {
	for (int i = 0; i < cycleCount; i++) {
		// dpSend.setDecimals(0, 0.5);
		dpSend.setCharacters(0, 'c');
		c_network.dpSend(dpSend);
		// std::cout << "Hello?" << std::endl;
		// const char buffer[DEFAULT_BUFFER_SIZE] = "Hello from client";
		// buffer = (char *)malloc(sizeof(char) * DEFAULT_BUFFER_SIZE);
		// buffer = "Hello from client";
		// c_network.raw_send(buffer);
		// free(buffer);
		// std::cout << "Whats the issue here" << std::endl;
		Sleep(1000);
	}
}

void divertCout(bool divert = true) {
	std::ofstream out("out.txt");
	std::streambuf *coutbuff = std::cout.rdbuf();
	std::cout.rdbuf(out.rdbuf());
}

int main() {
	std::cout << "Test Run..." << std::endl;
	// divertCout();

	// server();
	client();

	// std::thread server_t(server);
	// std::thread client_t(client);

	// server_t.join();
	// client_t.join();

	system("pause");

	return 0;
}