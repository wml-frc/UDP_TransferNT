#ifndef NETWORK_H
#define NETWORK_H

#include "Handshake.h"
#include "nt_headers.h"

class Network : public Serializer {
 public:
	enum class Type {
		SERVER = 0,
		CLIENT
	};

	enum class ConnectionType {
		ANY = 0,
		IP_SPECIFIC
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
	Network(Type t, ConnectionType ct, bool initHandShake = false) {
		_socketValues = new Socket();
		_type = new Type(t);
		_connectionType = new ConnectionType(ct);
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
	 * non-threaded sender for network
	 */
	void send(DataPacket *dp);

	/**
	 * non-threaded receive for network
	 */
	void recv(DataPacket *dp);

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

	ConnectionType getConnectionType() {
		return *_connectionType;
	}

 private:
	Socket *_socketValues;
	State *_state;
	ThreadState *_state_t;
	Type *_type;
	ConnectionType *_connectionType;

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