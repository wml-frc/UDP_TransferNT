#ifndef CLIENT_H
#define CLIENT_H

#include "NetworkContainer.h"

class Client : public Network {
 public:
	Client(ConnectionType ct = ConnectionType::IP_SPECIFIC, bool handshake = false) : Network(Type::CLIENT, ct, handshake) {
		std::cout << "UDP Client Created" << std::endl;
	}

	Type getType() { return Network::getType(); }
	ConnectionType getConnectionType() { return Network::getConnectionType(); }
};

#endif