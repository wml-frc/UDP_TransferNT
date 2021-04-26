#include "main.h"
#include <chrono>
#include <ctime>

Server server;
Client client(Network::ConnectionType::IP_SPECIFIC);

void server_func(bool sender = false) {
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

		if (sender) {
			dp.setCharacters(0, 't');
			dp.setIntegers(0, 3);
			dp.setBooleans(0, true);
			dp.setDecimals(0, 1.004);
			server.send(&dp);
		} else {
			server.recv(&dp);
			std::cout << "Received from Client" << std::endl;
			std::cout << "Server recv char: " << dp.getCharacters(0) << std::endl;
			std::cout << "Server recv int: " << dp.getIntegers(0) << std::endl;
			std::cout << "Server recv bool: " << dp.getBooleans(0) << std::endl;
			std::cout << "Server recv decimal: " << dp.getDecimals(0) << std::endl;
			system("clear");
		}
		// std::cout << "dt: " << dt.count() << std::endl;
		lastTime = currTime;
	}
}

void client_func(bool sender = false) {
	// Client client;
	// client.client_test();

	auto currTime = std::chrono::system_clock::now();
	std::chrono::duration<double> dt;
	auto lastTime = std::chrono::system_clock::now();

	// client.getSocket()->setPort(8080);
	client.getSocket()->setIP("192.168.178.125");
	client.init();
	std::cout << "Connected Client" << std::endl;

	sleep(5);

	DataPacket dp;

	while (true) {
		currTime = std::chrono::system_clock::now();
		dt = currTime-lastTime;

		if (sender) {
			dp.setCharacters(0, 't');
			dp.setIntegers(0, 3);
			dp.setBooleans(0, true);
			dp.setDecimals(0, 1.004);
			client.send(&dp);
		} else {
			client.recv(&dp);
			std::cout << "Received from Server" << std::endl;
			std::cout << "Client recv char: " << dp.getCharacters(0) << std::endl;
			std::cout << "Client recv int: " << dp.getIntegers(0) << std::endl;
			std::cout << "Client recv bool: " << dp.getBooleans(0) << std::endl;
			std::cout << "Client recv decimal: " << dp.getDecimals(0) << std::endl;
			system("clear");
		}

		// std::cout << "dt: " << dt.count() << std::endl;
		// system("clear");
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

	#ifdef CLIENT_RUN
	std::cout << "Client type: " << (int)client.getType() << std::endl;
	#endif

	#ifdef SERVER_RUN
		std::cout << "Server type: " << (int)server.getType() << std::endl;
	#endif
	
	#ifdef CLIENT_RUN
	std::thread client_t(client_func, true);
	#endif

	#ifdef SERVER_RUN
	std::thread server_t(server_func, false);
	#endif


	#ifdef CLIENT_RUN
	client_t.join();
	#endif

	#ifdef SERVER_RUN
	server_t.join();
	#endif




	return 0;
}