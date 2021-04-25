#include "NetworkContainer.h"

int Network::init() {

	/**
	 * Clear addresses
	 */
	bzero(_socketValues->getLocalAddress(), sizeof(*_socketValues->getLocalAddress()));

	/**
	 * Create UDP socket
	 */
	if ((*_socketValues->getSocket() = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		KILL("SOCKET ERROR");
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
			_socketValues->getExternalAddress()->sin_addr.s_addr = inet_addr(_socketValues->getIP()); // Connect to IP address
			break;
	}

	/**
	 * Bind/Connect the socket
	 */
	switch (*this->_type) {
		case Type::SERVER:
			if (bind(*_socketValues->getSocket(), (struct sockaddr *)_socketValues->getLocalAddress(), sizeof(*_socketValues->getLocalAddress())) == -1) {
				KILL("BIND ERROR");
			}
			break;
		case Type::CLIENT:
			if (connect(*_socketValues->getSocket(), (struct sockaddr *)_socketValues->getExternalAddress(), *_socketValues->getExternalAddressLen()) == -1) {
				KILL("CONNECT FAIL");
			}
			break;
	}

	/**
	 * Handshake (gets buffersize)
	 */
	// handShake();
}

/**
 * Send data to ip and port
 */
void Network::send(DataPacket *dp) {
	char buffer[DEFAULT_BUFFSIZE];

	serialize(dp, buffer);

	switch (*this->_type) {
		case Type::SERVER:
			if (sendto(*_socketValues->getSocket(), buffer, DEFAULT_BUFFSIZE, 0, (struct sockaddr *)_socketValues->getExternalAddress(), *_socketValues->getExternalAddressLen()) == -1) {
				KILL("SEND");
			}
			break;
		case Type::CLIENT:
			if (sendto(*_socketValues->getSocket(), buffer, DEFAULT_BUFFSIZE, 0, (struct sockaddr *)NULL, *_socketValues->getExternalAddressLen()) == -1) {
				KILL("SEND");
			}
			break;
	}

	memset(buffer, 0, sizeof(buffer));
}

/**
 * Receive data from ip and port
 */
void Network::recv(DataPacket *dp) {
	char buffer[DEFAULT_BUFFSIZE];

	if ((*_socketValues->getRecvLen() = recvfrom(*_socketValues->getSocket(), buffer, DEFAULT_BUFFSIZE, 0, (struct sockaddr *)_socketValues->getExternalAddress(), _socketValues->getExternalAddressLen())) == -1) {
		KILL("RECEIVE");
	}

	switch (*this->_type) {
		case Type::SERVER:
			*_socketValues->getValread() = recvfrom(*_socketValues->getSocket(), buffer, sizeof(buffer), 0, (struct sockaddr *)_socketValues->getExternalAddress(), _socketValues->getExternalAddressLen());
			if (*_socketValues->getValread() == -1) { KILL("RECV"); }
			break;
		case Type::CLIENT:
			*_socketValues->getValread() = recvfrom(*_socketValues->getSocket(), buffer, sizeof(buffer), 0, (struct sockaddr *)NULL, NULL);
			break;
	}

	deserialize(dp, buffer);
	memset(buffer, 0, sizeof(buffer));
}

/**
 * @TODO: Make threaded network (Time and a half f)
 */
void Network::update() {
	switch(*_state) {
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