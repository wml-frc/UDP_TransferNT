#include "NetworkContainer.h"

namespace UDP_TransferNT {
	#ifdef __linux__

	void Network::initNetwork() {

		/**
		 * Set state: Connecting
		 */
		setState(State::CONNECTING);

		struct hostent *host;
		host = (struct hostent *)gethostbyname((char *)_socketValues->getIP());

		/**
		 * Create UDP socket
		 */
		if ((*_socketValues->getSocket() = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
			setState(State::DEAD);
			KILL("SOCKET ERROR");
		}

		/**
		 * Memset
		 */
		if (*this->_connectionType == ConnectionType::ANY) {
			memset(_socketValues->getLocalAddress(), 0, *_socketValues->getLocalAddressLen());
			memset(_socketValues->getExternalAddress(), 0, *_socketValues->getExternalAddressLen());
		}

		/**
		 * Address set
		 */
		switch (*this->_type) {
			case Type::SERVER:
				_socketValues->getLocalAddress()->sin_family = AF_INET;
				_socketValues->getLocalAddress()->sin_port = htons(*_socketValues->getPort());
				_socketValues->getLocalAddress()->sin_addr.s_addr = htonl(INADDR_ANY); // Allow any connecting addresses
				break;
			case Type::CLIENT:
				_socketValues->getExternalAddress()->sin_family = AF_INET;
				_socketValues->getExternalAddress()->sin_port = htons(*_socketValues->getPort());
				switch (*this->_connectionType) {
					case ConnectionType::ANY:
						_socketValues->getExternalAddress()->sin_addr.s_addr = INADDR_ANY;
						break;
					case ConnectionType::IP_SPECIFIC:
						_socketValues->getExternalAddress()->sin_addr = *((struct in_addr *)host->h_addr);
						break;
				}
				break;
		}


		/**
		 * Clear addresses
		 */
		if (*this->_connectionType == ConnectionType::IP_SPECIFIC) {
			bzero(_socketValues->getLocalAddress()->sin_zero, 8);
			bzero(_socketValues->getExternalAddress()->sin_zero, 8);
		}

		/**
		 * Bind/Connect the socket
		 */
		switch (*this->_type) {
			case Type::SERVER:
				if (bind(*_socketValues->getSocket(), (struct sockaddr *)_socketValues->getLocalAddress(), *_socketValues->getLocalAddressLen()) < 0) {
					setState(State::DEAD);
					KILL("BIND ERROR");
				}
				break;
			case Type::CLIENT:
				switch (*this->_connectionType) {
					case ConnectionType::ANY:
						break;
					case ConnectionType::IP_SPECIFIC:
						if (connect(*_socketValues->getSocket(), (struct sockaddr *)_socketValues->getExternalAddress(), *_socketValues->getExternalAddressLen()) < 0) {
							setState(State::DEAD);
							KILL("CONNECT FAIL");
						}
						break;
				}
				break;
		}

		setState(State::CONNECTED);
		setState_t(ThreadState::RUNNING);
	}

	/**
	 * @TODO: Make threaded network (Time and a half f)
	 */
	void Network::update() {
		switch(_state) {
			case State::IDLE:
				setState_t(ThreadState::IDLE); // Will continue to loop, but will not send or receive using socket values
				break;

			case State::CONNECTING:
				setState_t(ThreadState::IDLE);
				break;
			
			case State::CONNECTED:
				setState_t(ThreadState::RUNNING); // Run thread loops
				break;

			case State::DEAD:
				// Stop all processes in order
				setState_t(ThreadState::STOP);

				// Join threads before purging values
				if (send_t.joinable()) {
					send_t.join();
				}

				if (rec_t.joinable()) {
					rec_t.join();
				}

				// Purge
				close(*_socketValues->getSocket());
				*_socketValues->getSocket() = 0;
				memset((char *)_socketValues->getLocalAddress(), 0, sizeof(*_socketValues->getLocalAddress()));
				break;
			
			default:
				setState(State::IDLE);
				break;
		}
	}

	#endif
}