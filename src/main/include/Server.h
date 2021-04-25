#ifndef SERVER_H
#define SERVER_H

#include "NetworkContainer.h"

class Server : public Network {
 public:
	Server() : Network(Type::SERVER, false) {
		std::cout << "UDP Server Created" << std::endl;
	}

	Type getType() { return Network::getType(); }
};

#endif