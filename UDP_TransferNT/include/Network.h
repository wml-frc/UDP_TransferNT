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
		 * Connection type
		 * Meant for client connection to either a specific ip address or any ip address on the same port
		 */
		enum class ConnectionType {
			ANY = 0,
			IP_SPECIFIC
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
			RUNNING
		};

		/**
		 * Main network, initialize as either server or client
		 */
		Network(Type type, ConnectionType connType, int port = DEFAULT_NT_PORT, int bufferSize = DEFAULT_BUFFER_SIZE) : _type(type), _connType(connType), _port(port), _bufferSize(bufferSize) {
			DEFAULT_NT_LOGGER("Network Constructed with values");
			DEFAULT_NT_LOGGER("Type: " + std::to_string((int)_type));
			DEFAULT_NT_LOGGER("Connection Type: " + std::to_string((int)_connType));
			DEFAULT_NT_LOGGER("Port: " + std::to_string(_port));
			DEFAULT_NT_LOGGER("Buffer Size: " + std::to_string(_bufferSize));
		}

		void init() {
			while (_connStat != ConnectionStatus::CONNECTED) {
				int programValue = 0;
				if (_connStat == ConnectionStatus::CONNECTING) {
					DEFAULT_NT_LOGGER("Closing Socket, reconnecting...");
					_socket.close();
				}

				_connStat = ConnectionStatus::CONNECTING;

				DEFAULT_NT_LOGGER("Opening socket...");

				switch (_type) {
					case Type::SERVER: // SERVER
						if (programValue + _socket.createSocket() != 0) {
							DEFAULT_NT_LOGGER("Error opening socket");
						}

						if (_connType == ConnectionType::ANY) {
							_socket.prepSocketStructure();
						} else if (_connType == ConnectionType::IP_SPECIFIC) {
							_socket.prepSocketStructure(false, true);
						}

						if (programValue + _socket.bindSocket() != 0) {
							DEFAULT_NT_LOGGER("Error binding socket");
						}
						break; // SERVER END

					case Type::CLIENT: // CLIENT
						if (programValue + _socket.createSocket(true) != 0) {
							DEFAULT_NT_LOGGER("Error opening socket");
						}

						if (_connType == ConnectionType::ANY) {
							_socket.prepSocketStructure(true);
						} else if (_connType == ConnectionType::IP_SPECIFIC) {
							_socket.prepSocketStructure(true, true);
						}

						break; // CLIENT END
				}
			}
		}


		void raw_send() {}
		void raw_recv() {}

		void dpSend() {}
		void dpRecv() {}

	 private:
		Type _type;
		ConnectionType _connType{ ConnectionType::ANY };
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