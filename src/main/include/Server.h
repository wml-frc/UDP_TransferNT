#ifndef SERVER_H
#define SERVER_H

#include "NetworkContainer.h"

class Server : public Network {
 public:
	Server(bool thread = false) : Network(Type::SERVER, ConnectionType::IP_SPECIFIC), _thread(thread) {
		std::cout << "UDP Server Created" << std::endl;
	}

	Type getType() { return Network::getType(); }

	void init();

 private:
	bool _thread;

	void init_thread();
};

#endif