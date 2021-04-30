#ifndef CLIENT_H
#define CLIENT_H

#include "NetworkContainer.h"

namespace UDP_TransferNT {
	#ifdef __linux__
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

	#else

	class Client : public Network {
	 public:
		Client(bool thread = false, ConnectionType ct = ConnectionType::IP_SPECIFIC, bool handshake = false) : _thread(thread), Network(Type::CLIENT, ct) {
			std::cout << "UDP Client runs only on linux" << std::endl;
		}

		int getType() { return 0; }
		int getConnectionType() { return 0; }

		void init() {}
		void send(DataPacket *dp) {}
		void recv(DataPacket *dp) {}

	private:
		bool _thread;
	};
	#endif
}


#endif