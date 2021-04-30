#include "NetworkContainer.h"

namespace UDP_TransferNT {
	#ifdef __linux__

	/**
	 * Send data to ip and port
	 */
	void Network::send(DataPacket *dp) {
		if (this->getState() == State::DEAD) {
			close(*this->getSocket()->getSocket());
			return;
		}

		if (this->getState() == State::CONNECTED) {
			if (this->getState_t() == ThreadState::RUNNING) {
				char buffer[PACKETSIZE];
				serialize(dp, buffer);


				switch (*this->_type) {
					case Type::SERVER:
						if (sendto(*_socketValues->getSocket(), buffer, sizeof(buffer), 0, (const struct sockaddr *)_socketValues->getExternalAddress(), *_socketValues->getExternalAddressLen()) < 0) {
							setState(State::DEAD);
							ERROR_PRINT("SEND SERVER");
						}
						break;
					case Type::CLIENT:
						switch (*this->_connectionType) {

							case ConnectionType::ANY:
								if (sendto(*_socketValues->getSocket(), buffer, sizeof(buffer), 0, (const struct sockaddr *)_socketValues->getExternalAddress(), *_socketValues->getExternalAddressLen()) < 0) {
									setState(State::DEAD);
									ERROR_PRINT("SEND CLIENT");
								}
								break;

								case ConnectionType::IP_SPECIFIC:
									if (sendto(*_socketValues->getSocket(), buffer, sizeof(buffer), 0, (const struct sockaddr *)NULL, *_socketValues->getExternalAddressLen()) < 0) {
										setState(State::DEAD);
										ERROR_PRINT("SEND CLIENT");
									}
									break;
						}
						break;
				}

				memset(buffer, 0, sizeof(buffer));
			}
		} else {
			// std::cout << "Cannot send, not connected" << std::endl;
			return;
		}
	}

	void Network::sendThreadFunc(DataPacket *dp) {
		while (this->getState() != State::DEAD) {
			if (this->getState_t() != ThreadState::IDLE) {
				this->send(dp);
			}
		}
	}

	void Network::registerSend(DataPacket *dp) {
		std::thread send_t(&Network::sendThreadFunc, this, dp);
		send_t.detach();
	}

	#endif
}