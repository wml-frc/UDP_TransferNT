#include "main.h"
#include <chrono>
#include <ctime>

Server server(true);
Client client(true, Network::ConnectionType::IP_SPECIFIC, true);

void server_func(bool sender = false) {
	
	auto currTime = std::chrono::system_clock::now();
	std::chrono::duration<double> dt;
	auto lastTime = std::chrono::system_clock::now();

	server.init();
	return;
	std::cout << "Connected Server" << std::endl;

	// sleep(5);

	DataPacket dp;

	float val = 1;

	while (true) {
		currTime = std::chrono::system_clock::now();
		dt = currTime-lastTime;

		if (sender) {
			dp.setCharacters(0, 't');
			dp.setIntegers(0, 3);
			dp.setBooleans(0, true);
			dp.setDecimals(0, val);
			server.send(&dp);
		} else {
			server.recv(&dp);
			std::cout << "Received from Client" << std::endl;
			std::cout << "Client State: " << (int)client.getState() << std::endl;
			std::cout << "Client Thread State: " << (int)client.getState_t() << std::endl;

			std::cout << "Server recv char: " << dp.getCharacters(0) << std::endl;
			std::cout << "Server recv int: " << dp.getIntegers(0) << std::endl;
			std::cout << "Server recv bool: " << dp.getBooleans(0) << std::endl;
			std::cout << "Server recv decimal: " << dp.getDecimals(0) << std::endl;
			std::cout << "dt: " << dt.count() << std::endl;
			system("clear");
		}
		val += 0.001;
		lastTime = currTime;
	}
}

void client_func(bool sender = false) {

	auto currTime = std::chrono::system_clock::now();
	std::chrono::duration<double> dt;
	auto lastTime = std::chrono::system_clock::now();

	// client.getSocket()->setPort(8080);
	// client.getSocket()->setIP(""); // 192.168.178.125
	client.init();
	return;
	std::cout << "Connected Client" << std::endl;

	DataPacket dp;

	float val = 1;

	while (true) {
		currTime = std::chrono::system_clock::now();
		dt = currTime-lastTime;

		if (sender) {
			dp.setCharacters(0, 't');
			dp.setIntegers(0, 3);
			dp.setBooleans(0, true);
			dp.setDecimals(0, val);
			client.send(&dp);
		} else {
			client.recv(&dp);
			std::cout << "Received from Server" << std::endl;
			std::cout << "Client recv char: " << dp.getCharacters(0) << std::endl;
			std::cout << "Client recv int: " << dp.getIntegers(0) << std::endl;
			std::cout << "Client recv bool: " << dp.getBooleans(0) << std::endl;
			std::cout << "Client recv decimal: " << dp.getDecimals(0) << std::endl;
			std::cout << "dt: " << dt.count() << std::endl;
			system("clear");
		}

		// system("clear");
		val += 0.001;
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

	DataPacket dpSend, dpRec;
	dpSend.setDecimals(0, 1);

	client.registerSend(&dpSend);
	server.registerRecv(&dpRec);

	float value = 0;
	while(true) {
		std::cout << "Received from client: " << dpRec.getDecimals(0) << std::endl;
		dpSend.setDecimals(0, value);
		value += 0.0001;
		sleep(0.5);
	}

	// sleep(10);
	// client.stop();
	// sleep(10);
	// client.start();
	// sleep(10);
	// client.kill();


	#ifdef CLIENT_RUN
	client_t.join();
	#endif

	#ifdef SERVER_RUN
	server_t.join();
	#endif




	return 0;
}