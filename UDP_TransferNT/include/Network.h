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
		Network(Type type, ConnectionType connType, int port = DEFAULT_NT_PORT, const char *ip = DEFAULT_NT_IP, int bufferSize = DEFAULT_BUFFER_SIZE) : _type(type), _connType(connType),  _bufferSize(bufferSize) {
			// Set the socket vals
			_socket.setPort(port);
			_socket.setIP(ip);

			// Display the vals
			DEFAULT_NT_LOGGER("Network Constructed with values");
			DEFAULT_NT_LOGGER("Type: " + std::to_string((int)_type));
			DEFAULT_NT_LOGGER("Connection Type: " + std::to_string((int)_connType));
			DEFAULT_NT_LOGGER("Port: " + std::to_string(_socket.getPort()));
			DEFAULT_NT_LOGGER(_socket.getIP());
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

				if (programValue == 0) {
					_connStat = ConnectionStatus::CONNECTED;
					_state = State::IDLE;
				}
			}
		}

		/**
		 * Reset network
		 * Closes socket and re-runs the connection process
		 */
		void reset() {
			_state = State::UN_INITIALIZED;
			_connStat = ConnectionStatus::CONNECTING;
			init();
		}


		/**
		 * Raw send
		 * Send a buffer with max size of the bufferSize
		 */
		int raw_send(char *buffer) {
			_state = State::RUNNING;
			int programValue = 0;
			switch (_type) {
				case Type::SERVER:
					#ifdef NT_UDP_PLATFORM_WINDOWS
					if (sendto(_socket.getSocket(), buffer, _bufferSize, 0, (struct sockaddr *)&_socket.getOtherAddress(), _socket.getOtherAddressLength()) == SOCKET_ERROR) {
						DEFAULT_NT_LOGGER("Send error");
						programValue = 1;
					}
					#elif defined(NT_UPD_PLATFORM_UNIX)
					if (sendto(_socket.getSocket(), buffer, _bufferSize, 0, (struct sockaddr *)&_socket.getOtherAddress(), _socket.getOtherAddressLength()) < 0) {
						DEFAULT_NT_LOGGER("Send error");
						programValue = 1;
					}
					#endif
					break;

				case Type::CLIENT:
					#ifdef NT_UDP_PLATFORM_WINDOWS
					if (sendto(_socket.getSocket(), buffer, _bufferSize, 0, (struct sockaddr *)&_socket.getOtherAddress(), _socket.getOtherAddressLength()) == SOCKET_ERROR) {
						DEFAULT_NT_LOGGER("Send error");
						programValue = 1;
					}
					#elif defined(NT_UPD_PLATFORM_UNIX)
					if (sendto(_socket.getSocket(), buffer, _bufferSize, 0, (struct sockaddr *)&_socket.getOtherAddress(), _socket.getOtherAddressLength()) < 0) {
						DEFAULT_NT_LOGGER("Send error");
						programValue = 1;
					}
					#endif
					break;
			}
			_state = State::IDLE;
			return programValue;
		}

		/**
		 * Raw receive
		 * receive a raw buffer with a max buffersize
		 */
		std::pair<char *, int> raw_recv() {
		// char *raw_recv() {
			_state = State::RUNNING;
			char *buffer;
			int programValue = 0;
			switch (_type) {
				case Type::SERVER:
					#ifdef NT_UDP_PLATFORM_WINDOWS
					if (recvfrom(_socket.getSocket(), buffer, _bufferSize, 0, (struct sockaddr *)&_socket.getOtherAddress(), &_socket.getOtherAddressLength()) == SOCKET_ERROR) {
						DEFAULT_NT_LOGGER("Recv error");
						programValue = 1;
					}
					#elif defined(NT_UDP_PLATFORM_UNIX)
					if (recvfrom(_socket.getSocket(), buffer, _bufferSize, 0, (struct sockaddr *)&_socket.getOtherAddress(), &_socket.getOtherAddressLength()) < 0) {
						DEFAULT_NT_LOGGER("Recv error");
						programValue = 1;
					}
					#endif
					break;

				case Type::CLIENT:
					#ifdef NT_UDP_PLATFORM_WINDOWS
					if (recvfrom(_socket.getSocket(), buffer, _bufferSize, 0, (struct sockaddr *)&_socket.getOtherAddress(), &_socket.getOtherAddressLength()) == SOCKET_ERROR) {
						DEFAULT_NT_LOGGER("Recv error");
						programValue = 1;
					}
					#elif defined(NT_UDP_PLATFORM_UNIX)
					if (recvfrom(_socket.getSocket(), buffer, _bufferSize, MSG_DONTWAIT, (struct sockaddr *)&_socket.getOtherAddress(), &_socket.getOtherAddressLength()) < 0) {
						DEFAULT_NT_LOGGER("Recv error");
						programValue = 1;
					}
					#endif
					break;
			}
			_state = State::IDLE;
			return {buffer, programValue};
		}

		/**
		 * Send over a serialized datapacket
		 */
		void dpSend(DataPacket dp) {
			if (_connStat == ConnectionStatus::CONNECTED) {
				char *buffer;
				serialize(dp, buffer);
				raw_send(buffer);
			}
		}

		/**
		 * receive a deserialized datapacket
		 * Use previously received datapacket as paramater to get streamline data.
		 * (Stops random zeroed out datapacket from being returned if no datapacket is received)
		 */
		DataPacket dpRecv(DataPacket dpPrevious = {0}) {
			DataPacket dpBuffer;
			std::pair<char *, int> recvBuffer;
			if (_connStat == ConnectionStatus::CONNECTED) {
				recvBuffer = raw_recv();
				if (recvBuffer.second == 0) {
					deserialize(dpBuffer, recvBuffer.first);
					return dpBuffer;
				}
			}

			return dpPrevious;
		}

	 private:
		Type _type;
		ConnectionType _connType{ ConnectionType::ANY };
		const int _bufferSize;

		// Socket
		Socket _socket;

		// connections and states
		ConnectionStatus _connStat{ ConnectionStatus::DISCONNECTED };
		State _state{ State::UN_INITIALIZED };
	};
}

#endif // NETWORK_H