#include "Server.h"

void Server::init_thread() {
	this->initNetwork();
}

void Server::init() {
	std::cout << "Main Socket Port: " << *this->getSocket()->getPort() << std::endl;

	if (_thread) {
		std::cout << "Threaded start enabled" << std::endl;
		std::thread init_t(&Server::init_thread, this);
		init_t.detach();
	} else {
		this->initNetwork();
	}
}	