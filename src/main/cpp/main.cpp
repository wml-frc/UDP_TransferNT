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

	std::thread server_t(server);
	std::thread client_t(client);

	// server.server_test();
	// client.client_test();

	server_t.join();
	client_t.join();

	return 0;
}