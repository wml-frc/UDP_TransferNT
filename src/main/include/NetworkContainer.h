#ifndef SOCKET_CONTAINER_H
#define SOCKET_CONTAINER_H

#include "nt_headers.h"

/**
 * Socket values used for connecting
 */
class Socket {
 public:
	/**
	 * Getters (Pointer functions can be used to set values)
	 */
	uint16_t *getHandshakePort() { return &h_port; }
	uint16_t *getPort() { return &port; }
	char *getIP() { return ipaddress; }
	int *getSocket() { return &sock; }
	int *getValread() { return &valread; }
	struct sockaddr_in *getLocalAddress() { return &address_local; }
	struct sockaddr_in *getExternalAddress() { return &address_local; }
	socklen_t *getExternalAddressLen() { return &addressExt_len; }

 private:
	uint16_t h_port = HANDSHAKE_PORT;
	uint16_t port = PORT;
	char ipaddress[20] = SERVER_IP;
	int sock = 0;
	int valread;
	struct sockaddr_in address_local, address_external;
	socklen_t addressExt_len = sizeof(address_external);
	bool InitHandshake = false;
};

/**
 * Data packet for handshake
 */
struct DataPacket_H {
	// Simple check for data
	bool dataTrue = false;
	int data_buffersize = 0;
	char version[sizeof(NETWORK_VERSION)] = NETWORK_VERSION;
};

#define PACKETSIZE_H sizeof(DataPacket_H)


/**
 * Data packet for user data
 */
struct DataPacket {
	char characters[BUFFSIZE]{0};
	int integers[BUFFSIZE]{0};
	bool booleans[BUFFSIZE]{0};
	double doubles[BUFFSIZE]{0};
};

#define PACKETSIZE sizeof(DataPacket)

class Network {
 public:
	enum class Type {
		SERVER = 0,
		CLIENT
	};

	enum class ThreadState {
		STOP = 0,
		IDLE,
		RUNNING
	};
	
	enum class State {
		IDLE = 0,
		CONNECTING,
		CONNECTED,
		DEAD
	};

	/**
	 * Construct a network with type and default state
	 */
	Network(Type t, bool initHandShake = false) {
		_socketValues = new Socket();
		_type = new Type(t);
		_state = new State(State::IDLE);
		_handShaker = initHandShake;
	}

	/**
	 * Intialize the connection, also choose if the server initializes handshake or client, by default the server does it
	 */
	int init();

	/**
	 * Main updater for network
	 */
	void update();

	/**
	 * KILL:connection, purges values
	 */
	void kill() {
		setState(State::DEAD); // Kills all loops and processes and purges socket
	}

	/**
	 * STOP:Connection, does not purge values
	 */
	void stop() {}

	/**
	 * START:Connection, or restart connection again
	 */
	void start() {}

	/**
	 * Get socket
	 */
	Socket *getSocket() {
		return _socketValues;
	}

 protected:
	void handShake();

	/**
	 * Set State
	 */
	void setState(State st) {
		*_state = st;
	}

	State getState() {
		return *_state;
	}

	Type getType() {
		return *_type;
	}

 private:
	Socket *_socketValues;
	State *_state;
	ThreadState *_state_t;
	Type *_type;

	bool _handShaker = false; // Are we the shaker? or the shakee? Programming jokes... love em

	std::thread send_t; // send thread
	std::thread rec_t; // receive thread

	void setState_t(ThreadState st) {
		*_state_t = st;
	}

	ThreadState getState_t() {
		return *_state_t;
	}
};

#endif