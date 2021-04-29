#include "NetworkContainer.h"

/**
 * Receive data from ip and port
 */
void Network::recv(DataPacket *dp) {
	if (this->getState() == State::CONNECTED) {
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
	} else {
		std::cout << "Cannot Receive, not connected" << std::endl;
	}
}