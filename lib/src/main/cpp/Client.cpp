#include "Client.h"

namespace UDP_TransferNT {
	#ifdef __linux__

	void Client::init_thread() {
		this->initNetwork();
	}

	void Client::init() {
			std::cout << "Main Socket Port: " << *this->getSocket()->getPort() << std::endl;

		if (_thread) {
			std::cout << "Threaded start enabled" << std::endl;
			std::thread init_t(&Client::init_thread, this);
			init_t.detach();
		} else {
			this->initNetwork();
		}
	}
	#endif
}