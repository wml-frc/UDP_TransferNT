#include "main.h"

int server() {
	Server server;
	server.server_test();
}

int client() {
	Client client;
	client.client_test();
}

int main() {
	// Server server;
	// Client client;

	// std::thread client_t(client);
	// std::thread server_t(server);

	// server.server_test();
	// client.client_test();

	// server_t.join();
	// client_t.join();
	std::cout << "Test" << std::endl;

	return 0;
}