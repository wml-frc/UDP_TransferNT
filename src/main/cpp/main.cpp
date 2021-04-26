#include "main.h"
#include <chrono>
#include <ctime>

Server server;
Client client;

void server_func() {
	// Server server;
	// server.server_test();
	
	auto currTime = std::chrono::system_clock::now();
	std::chrono::duration<double> dt;
	auto lastTime = std::chrono::system_clock::now();

	server.init();
	std::cout << "Connected Server" << std::endl;

	// sleep(5);

	DataPacket dp;

	while (true) {
		currTime = std::chrono::system_clock::now();
		dt = currTime-lastTime;

		dp.setCharacters(0, 't');
		dp.setIntegers(0, 3);
		dp.setBooleans(0, true);
		dp.setDecimals(0, 1.004);
		server.send(&dp);

		std::cout << "Server send char: " << dp.getCharacters(0) << std::endl;
		std::cout << "Server send int: " << dp.getIntegers(0) << std::endl;
		std::cout << "Server send bool: " << dp.getBooleans(0) << std::endl;
		std::cout << "Server send decimal: " << dp.getDecimals(0) << std::endl;

		std::cout << "dt: " << dt.count() << std::endl;
		system("clear");
		lastTime = currTime;
	}
}

void client_func() {
	// Client client;
	// client.client_test();

	auto currTime = std::chrono::system_clock::now();
	std::chrono::duration<double> dt;
	auto lastTime = std::chrono::system_clock::now();

	client.getSocket()->setIP("127.0.0.1");
	client.init();
	std::cout << "Connected Client" << std::endl;

	// sleep(5);

	DataPacket dp;

	while (true) {
		currTime = std::chrono::system_clock::now();
		dt = currTime-lastTime;

		server.recv(&dp);
		
		std::cout << "Server Address IP: " << client.getSocket()->getIP() << " PORT: " << client.getSocket()->getPort() << std::endl;
		std::cout << "Client recv char: " << dp.getCharacters(0) << std::endl;
		std::cout << "Client recv int: " << dp.getIntegers(0) << std::endl;
		std::cout << "Client recv bool: " << dp.getBooleans(0) << std::endl;
		std::cout << "Client recv decimal: " << dp.getDecimals(0) << std::endl;

		std::cout << "dt: " << dt.count() << std::endl;
		system("clear");
		lastTime = currTime;
	}
}

int main() {
	// Server server;
	// Client client;

	// std::thread client_t(client);
	// std::thread server_t(server);

	// server.server_test();
	// client.client_test();

	// int16_t value = 5;
	// char16_t character = 'h';
	// _Float32 doubleVal = 0.001;

	// server_t.join();
	// client_t.join();


	std::cout << "Test" << std::endl;

	std::cout << "Server type: " << (int)server.getType() << std::endl;
	std::cout << "Client type: " << (int)client.getType() << std::endl;

	std::thread server_t(server_func);
	std::thread client_t(client_func);

	server_t.join();
	client_t.join();

	return 0;
}