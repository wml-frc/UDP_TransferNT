#ifndef SOCKET_H
#define SOCKET_H

#include "nt_headers.h"

namespace UDP_TransferNT {

	/**
	 * Cross platform socket wrapper. Used as main socket for network.
	 */
	class Socket {
	 public:
		void setPort(int port) {
			_port = port;
		}

		void setIP(const char *ip) {
			_ip = ip;
		}

		int getPort() {
			return _port;
		}

		const char *getIP() {
			return _ip;
		}

		struct sockaddr_in &getLocalSocket() {
			return _si_local;
		}

		struct sockaddr_in &getOtherSocket() {
			return _si_other;
		}

		int createSocket(bool client = false) {

			// windows create socket
			#ifdef NT_UDP_PLATFORM_WINDOWS
				if (WSAStartup(MAKEWORD(2,2), &_wsa) != 0) {
					DEFAULT_NT_LOGGER("WSA Startup Failed with error: " + std::to_string(WSAGetLastError()));
					return 1;
				}

				if (client) {
					if ((_socket = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == SOCKET_ERROR) {
						DEFAULT_NT_LOGGER("Could not create client socket, error: " + std::to_string(WSAGetLastError()));
						return 1;
					}
				} else {
					if ((_socket = socket(AF_INET, SOCK_DGRAM, 0)) == INVALID_SOCKET) {
						DEFAULT_NT_LOGGER("Could not create server socket, error: " + std::to_string(WSAGetLastError()));
						return 1;
					}
				}

			// unix create socket
			#elif defined(NT_UDP_PLATFORM_UNIX)
				if (cleint) {
					// @TODO
				} else {
					if ((_socket = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
						DEFAULT_NT_LOGGER("Could not create client socket");
						return 1;
					}
				}
			#endif
			return 0;
		}

		void prepSocketStructure(bool client = false, bool ipSpecific = false) {

			// zero out addresses
			memset((char *)&_si_local, 0, sizeof(_si_local));
			memset((char *)&_si_other, 0, sizeof(_si_other));

			if (client) {
				_si_other.sin_family = AF_INET;
				_si_other.sin_port = htons(_port);
				if (ipSpecific) {
					#ifdef NT_UDP_PLATFORM_WINDOWS
						_si_other.sin_addr.S_un.S_addr = inet_addr(_ip);
					#elif defined(NT_UDP_PLATFORM_UNIX)
						_host = (struct hostent *)gethostbyname((char *)_ip);
						_si_other.sin_addr.s_addr = ((struct in_addr *)_host->h_addr);
					#endif
				} else {
					_si_other.sin_addr.s_addr = INADDR_ANY;
				}
			} else {
				_si_local.sin_family = AF_INET;
				_si_local.sin_port = htons(_port);
				_si_local.sin_addr.s_addr = INADDR_ANY;
			}
		}

		int bindSocket(bool client = false) {
			#ifdef NT_UDP_PLATFORM_WINDOWS
				if (bind(_socket, (struct sockaddr *)&_si_local, sizeof(_si_local)) == SOCKET_ERROR) {
					DEFAULT_NT_LOGGER("Bind failed, error: " + std::to_string(WSAGetLastError()));
					return 1;
				}
			#elif defined(NT_UDP_PLATFORM_UNIX)
				if (bind(_socket, (struct sockaddr *)&_si_local, sizeof(_si_local)) < 0) {
					DEFAULT_NT_LOGGER("Bind failed");
					return 1
				}
			#endif
			return 0;
		}

		int connectSocket(bool client = false) {
			#ifdef NT_UDP_PLATFORM_WINDOWS
			#elif defined(NT_UDP_PLATFORM_UNIX)
				if (connect(_socket, (struct sockaddr *)&_si_other, sizeof(_si_other)) < 0) {
					DEFAULT_NT_LOGGER("Socket Connect failed");
					return 1;
				}
			#endif
			return 0;
		}

		void close() {
			#ifdef NT_UDP_PLATFORM_WINDOWS
				closesocket(_socket);
				WSACleanup();
			#elif defined(NT_UDP_PLATFORM_UNIX)
				close(_socket);
			#endif
		}

	 private:
		int _port;
		const char *_ip;

		struct sockaddr_in _si_local, _si_other; // Server uses both socket addresses. Client uses si_other
		int _send_len, _recv_len;
		char BUFFER[DEFAULT_BUFFER_SIZE];

		#ifdef NT_UDP_PLATFORM_WINDOWS // windows
		SOCKET _socket;
		WSADATA _wsa;
		#elif defined(NT_UDP_PLATFORM_UNIX) // unix
		int _socket;
		struct hostent *_host;
		#endif
	};
}

#endif