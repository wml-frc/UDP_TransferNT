#ifndef SERVER_H
#define SERVER_H

#include "NetworkContainer.h"

class Server : public Network {
 public:
	Server(bool handshake = false) : Network(Type::SERVER, ConnectionType::IP_SPECIFIC, handshake) {
		std::cout << "UDP Server Created" << std::endl;
	}

	Type getType() { return Network::getType(); }
};

#endif