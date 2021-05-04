#ifndef SERVER_H
#define SERVER_H

#include "NetworkContainer.h"

namespace UDP_TransferNT {
	#ifdef __linux__
	class Server : public Network {
	 public:
		Server(bool thread = false) : Network(Type::SERVER, ConnectionType::IP_SPECIFIC) {
			this->_thread = thread;
			std::cout << "UDP Server Created" << std::endl;
		}

		Type getType() { return Network::getType(); }

		void init();

		private:
		bool _thread = false;

		void init_thread();
	};
	#else
	class Server : public Network {
	 public:
		Server(bool thread = false) : Network(Type::SERVER, ConnectionType::IP_SPECIFIC) {
			std::cout << "UDP Server runs only on linux" << std::endl;
			this->_thread = thread;
		}

		int getType() { return 0; }

		void init() {}
		void send(DataPacket *dp) {}
		void recv(DataPacket *dp) {}

		private:
		bool _thread = false;
	};
	#endif
}


#endif