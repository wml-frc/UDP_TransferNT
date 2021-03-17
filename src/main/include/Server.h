#ifndef SERVER_H
#define SERVER_H

#include "nt_headers.h"
#include "NetworkContainer.h"

class Server {
 public:
	enum class ServerState {
		IDLE = 0,
		CONNECTING,
		CONNECTED
	};
	Server();

	/**
	 * Intialize the connection, also choose if the server initializes handshake or client, by default the server does it
	 */
	int init(bool initHandshake = true);

	/**
	 * Update the server
	 */
	void update();

	/**
	 * Test server
	 */
	int server_test();

	/**
	 * Kill connection for server
	 */
	void kill() {
		close(*_socketValues->getSocket());
	}

	/**
	 * Start connection, or start connection again
	 */
	void start() {

	}

	Socket *getSocket() { return _socketValues; }
 private:
	Socket *_socketValues;
	ServerState *_state;

	/**
	 * Handshake
	 */
	void _handShake() {}

	/**
	 * Set Server State
	 */
	void setState(ServerState st) {
		*_state = st;
	}

	/**
	 * Get Server State
	 */
	ServerState getState() {
		return *_state;
	}
};

#endif