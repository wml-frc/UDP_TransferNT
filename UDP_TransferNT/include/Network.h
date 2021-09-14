#ifndef NETWORK_H
#define NETWORK_H

#include "Serializer.h"
#include "Socket.h"

namespace UDP_TransferNT {
	class Network : public Serializer {
	 public:

		/**
		 * Network Type
		 */
		enum class Type {
			SERVER = 0,
			CLIENT
		};

		/**
		 * Connection Status
		 */
		enum class ConnectionStatus {
			DISCONNECTED = 0,
			CONNECTING,
			CONNECTED
		};

		/**
		 * Network state
		 */
		enum class State {
			UN_INITIALIZED = 0,
			IDLE,
			NETWORK_DO
		};

		/**
		 * Main network, initialize as either server or client
		 */
		Network(Type type, int port = DEFAULT_NT_PORT, int bufferSize = DEFAULT_BUFFER_SIZE) : _type(type), _port(port), _bufferSize(bufferSize) {

		}

		void init() {
			while (_connStat != ConnectionStatus::CONNECTED) {
				if (_connStat == ConnectionStatus::CONNECTING) {
					DEFAULT_NT_LOGGER("Closing Socket, reconnecting...");
					_socket.close();
				}

				_connStat = ConnectionStatus::CONNECTING;

				DEFAULT_NT_LOGGER("Opening socket...");

				if (_socket.createSocket() != 0) {
					DEFAULT_NT_LOGGER("Error opening socket");
				}

				_socket.prepSocketStructure();
			}
		}

		int checkConn(bool forceReconn = false) {

		}

	 private:
		Type _type;
		int _port;
		int _bufferSize;

		// Socket
		Socket _socket;

		// connections and states
		ConnectionStatus _connStat{ ConnectionStatus::DISCONNECTED };
		State _state{ State::UN_INITIALIZED };
	};
}

#endif // NETWORK_H