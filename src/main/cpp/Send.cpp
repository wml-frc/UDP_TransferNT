#include "NetworkContainer.h"

/**
 * Send data to ip and port
 */
void Network::send(DataPacket *dp) {
	if (this->getState() == State::CONNECTED) {
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
	} else {
		std::cout << "Cannot send, not connected" << std::endl;
	}
}