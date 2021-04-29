#ifndef CLIENT_H
#define CLIENT_H

#include "NetworkContainer.h"

class Client : public Network {
 public:
	Client(bool thread = false, ConnectionType ct = ConnectionType::IP_SPECIFIC, bool handshake = false) : _thread(thread), Network(Type::CLIENT, ct) {
		std::cout << "UDP Client Created" << std::endl;
	}

	Type getType() { return Network::getType(); }
	ConnectionType getConnectionType() { return Network::getConnectionType(); }

	void init();

 private:
	bool _thread;

	void init_thread();
};

#endif