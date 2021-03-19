#include "NetworkContainer.h"

int Network::init() {
	/**
	 * Create UDP socket
	 */
	if ((*_socketValues->getSocket() = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		KILL("SOCKET ERROR");
	}

	/**
	 * Zero out structure
	 */
	memset((char *)_socketValues->getLocalAddress(), 0, sizeof(*_socketValues->getLocalAddress()));

	/**
	 * Address set
	 */
	_socketValues->getLocalAddress()->sin_family = AF_INET;
	_socketValues->getLocalAddress()->sin_port = htons(*_socketValues->getPort());
	_socketValues->getLocalAddress()->sin_addr.s_addr = htonl(INADDR_ANY); // Allow any connecting addresses

	/**
	 * Bind the socket
	 */
	if (bind(*_socketValues->getSocket(), (struct sockaddr *)_socketValues->getLocalAddress(), sizeof(*_socketValues->getLocalAddress())) == -1) {
		KILL("BIND ERROR");
	}

	/**
	 * Handshake (gets buffersize)
	 */
	// handShake();
}

void Network::send(DataPacket *dp) {
	char buffer[DEFAULT_BUFFSIZE];

	serialize(dp, buffer);

	if (sendto(*_socketValues->getSocket(), buffer, *_socketValues->getRecvLen(), 0, (struct sockaddr *)_socketValues->getExternalAddress(), *_socketValues->getExternalAddressLen()) == -1) {
		KILL("SEND");
	}

	memset(buffer, 0, sizeof(buffer));
}

void Network::recv(DataPacket *dp) {
	char buffer[DEFAULT_BUFFSIZE];

	if ((*_socketValues->getRecvLen() = recvfrom(*_socketValues->getSocket(), buffer, DEFAULT_BUFFSIZE, 0, (struct sockaddr *)_socketValues->getExternalAddress(), _socketValues->getExternalAddressLen())) == -1) {
		KILL("RECEIVE");
	}

	deserialize(dp, buffer);
} 

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