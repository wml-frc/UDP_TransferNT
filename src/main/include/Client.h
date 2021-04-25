#ifndef CLIENT_H
#define CLIENT_H

#include "NetworkContainer.h"

class Client : public Network {
 public:
	Client() : Network(Type::CLIENT, false) {
		std::cout << "UDP Client Created" << std::endl;
	}

	Type getType() { return Network::getType(); }
};

#endif