#include "NetworkContainer.h"

namespace UDP_TransferNT {
	#ifdef __linux__

	/**
	 * Receive data from ip and port
	 */
	void Network::recv(DataPacket *dp) {
		if (this->getState() == State::DEAD) {
			close(*this->getSocket()->getSocket());
			return;
		}

		if (this->getState() == State::CONNECTED) {
			if (this->getState_t() == ThreadState::RUNNING) {

				char buffer[PACKETSIZE];

				switch (*this->_type) {
					case Type::SERVER:
						*_socketValues->getValread() = recvfrom(*_socketValues->getSocket(), buffer, sizeof(buffer), 0, (struct sockaddr *)_socketValues->getExternalAddress(), _socketValues->getExternalAddressLen());
						if (*_socketValues->getValread() < 0) { 
							setState(State::DEAD);
							ERROR_PRINT("RECV SERVER"); 
						}
						break;
					case Type::CLIENT:

						switch (*this->_connectionType) {
							case ConnectionType::ANY:
									*_socketValues->getValread() = recvfrom(*_socketValues->getSocket(), buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)_socketValues->getExternalAddress(), _socketValues->getExternalAddressLen());
									if (*_socketValues->getValread() < 0) { 
										setState(State::DEAD);
										ERROR_PRINT("RECV CLIENT"); 
									}
								break;

							case ConnectionType::IP_SPECIFIC:
								*_socketValues->getValread() = recvfrom(*_socketValues->getSocket(), buffer, sizeof(buffer), 0, (struct sockaddr *)NULL, NULL);
								if (*_socketValues->getValread() < 0) { 
									setState(State::DEAD);
									ERROR_PRINT("RECV CLIENT"); 
								}
								break;
						}
						break;
				}

				deserialize(dp, buffer);

				memset(buffer, 0, sizeof(buffer));
			}
		} else {
			// std::cout << "Cannot Receive, not connected" << std::endl;
			return;
		}
	}

	void Network::recvThreadFunc(DataPacket *dp) {
		while(this->getState() != State::DEAD) {
			if (this->getState_t() != ThreadState::IDLE) {
				this->recv(dp);
			}
		}

		close(*this->getSocket()->getSocket());
		std::cout << "Receiver thread dead" << std::endl;
	}

	void Network::registerRecv(DataPacket *dp) {
		std::thread recv_t(&Network::recvThreadFunc, this, dp);
		recv_t.detach();
	}

	#endif
}