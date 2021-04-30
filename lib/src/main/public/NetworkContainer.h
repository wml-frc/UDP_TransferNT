#ifndef NETWORK_H
#define NETWORK_H

#include "nt_headers.h"
#include "Serializer.h"
#include "Socket.h"

namespace UDP_TransferNT {
	#ifdef __linux__
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
		Network(Type t, ConnectionType ct) {
			_socketValues = new Socket();
			_type = new Type(t);
			_connectionType = new ConnectionType(ct);
			this->setState(State::IDLE);
		}

		/**
		 * Intialize the connection, also choose if the server initializes handshake or client, by default the server does it
		 */
		void initNetwork();

		/**
		 * Main updater for network
		 */
		void update();

		/**
		 * non-threaded sender for network
		 */
		void send(DataPacket *dp);
		
		/**
		 * Threaded sender for network (call once, and it will continuously send data through datapacket address)
		 */
		void registerSend(DataPacket *dp);

		/**
		 * non-threaded receive for network
		 */
		void recv(DataPacket *dp);

		/**
		 * Threaded receiver for network (call once, and it will continuosly receive data through datapacket address)
		 */
		void registerRecv(DataPacket *dp);

		/**
		 * KILL:connection, purges values
		 */
		void kill() {
			setState(State::DEAD); // Kills all loops and processes and purges socket
		}

		/**
		 * STOP:Connection, does not purge values
		 */
		void stop() {
			this->setState_t(ThreadState::STOP);
			this->setState(State::IDLE);
		}

		/**
		 * START:Connection, or restart connection again
		 */
		void start() {
			this->setState_t(ThreadState::RUNNING);
			this->setState(State::CONNECTED);
		}

		/**
		 * Get socket
		 */
		Socket *getSocket() {
			return _socketValues;
		}

		/**
		 * Get current state of network
		 */
		State getState() {
			return _state;
		}

		/**
		 * Get current state of threads
		 */
		ThreadState getState_t() {
			return _state_t;
		}

	 protected:
		void handShake();

		/**
		 * Set State
		 */
		void setState(State st) {
			_state = st;
		}

		Type getType() {
			return *_type;
		}

		ConnectionType getConnectionType() {
			return *_connectionType;
		}

	 private:
		Socket *_socketValues;
		State _state;
		ThreadState _state_t;
		Type *_type;
		ConnectionType *_connectionType;

		// Handshake
		bool _handShaker = false; // Are we the shaker? or the shakee? Programming jokes... love em

		std::thread send_t; // send thread
		std::thread rec_t; // receive thread

		void setState_t(ThreadState st) {
			_state_t = st;
		}

		void recvThreadFunc(DataPacket *dp);
		void sendThreadFunc(DataPacket *dp);
	};
}

#endif