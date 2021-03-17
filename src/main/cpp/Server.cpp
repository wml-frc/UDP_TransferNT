#include "Server.h"

/**
 * Readdress the values
 */
Server::Server() {
	_socketValues = new Socket();
	_state = new ServerState(ServerState::IDLE);
}

int Server::init(bool initHandshake) {
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
	 * Handshake with client (Gets buffersize to send and recieve data)
	 */
	
}

void Server::update() {
	switch (*_state) {
		case ServerState::IDLE:
			break;
		
		case ServerState::CONNECTING:
			break;
		
		case ServerState::CONNECTED:
			break;

		default:
			break;
	}
}

int Server::server_test() {
	struct sockaddr_in si_me, si_other;

	int s, i, recv_len;
	socklen_t slen = sizeof(si_other);

	char buffer[BUFFSIZE];


	// create UDP socket
	if ((s=socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1) {
		// kill("socket");
		KILL("SOCKET");
	}

	// zero out the structure
	memset((char *) &si_me, 0, sizeof(si_me));

	si_me.sin_family = AF_INET;
	si_me.sin_port = htons(PORT);
	si_me.sin_addr.s_addr = htonl(INADDR_ANY);

	// bind socket to port
	if (bind(s, (struct sockaddr *)&si_me, sizeof(si_me)) == -1) {
		// kill("bind");
		KILL("SOCKET");
	}

	// keep listening for data
	while (true) {
		printf("Waiting for data...\n");
		fflush(stdout);

		// try to receive some data, this is a blocking call
		if ((recv_len = recvfrom(s, buffer, BUFFSIZE, 0, (struct sockaddr *)&si_other, &slen)) == -1) {
			// kill("recvfrom()");
			KILL("SOCKET");
		}

		// Print details of the client/peer and the data received
		printf("Received packet from %s:%d\n", inet_ntoa(si_other.sin_addr), ntohs(si_other.sin_port));
		printf("Data: %s\n", buffer);

		// now reply the client with same data
		if (sendto(s, buffer, recv_len, 0, (struct sockaddr *)&si_other, slen) == -1) {
			// kill("sendto()");
			KILL("SOCKET");
		}

		memset(buffer, 0, sizeof(buffer));
	}

	close(s);

	return 0;
}