#ifndef SOCKET_H
#define SOCKET_H
#include "nt_headers.h"
#include "DataPacket.h"

namespace UDP_TransferNT {
	#ifdef __linux__
	/**
	 * Socket values used for connecting
	 */
	class Socket {
	 public:
		/**
		 * Getters (Pointer functions can be used to set values)
		 */
		uint16_t *getHandshakePort() { return &h_port; }

		void setPort(uint16_t port) { this->port = port; }
		void setIP(const char ip[IP_LEN]) {
			for (int i = 0; i < IP_LEN; i++) {
				this->ipaddress[i] = ip[i];
			}
		}

		uint16_t *getPort() { return &port; }
		const char *getIP() { return ipaddress; }
		int *getSocket() { return &sock; }
		int *getValread() { return &valread; }
		socklen_t *getRecvLen() { return &recvlen; }
		struct sockaddr_in *getLocalAddress() { return &address_local; }
		struct sockaddr_in *getExternalAddress() { return &address_local; }

		socklen_t *getLocalAddressLen() { 
			addrLocLen = sizeof(address_local);
			return &addrLocLen;
		}
		socklen_t *getExternalAddressLen() { 
			addrExtLen = sizeof(address_external);
			return &addrExtLen; 
		}

		int16_t *getBufferSize() { return &bufferSize; }
		int16_t *getPacketSize() { return &packetSize; }

	 private:

		/**
		 * Socket Values
		 */
		uint16_t h_port = HANDSHAKE_PORT;
		uint16_t port = PORT;
		char ipaddress[IP_LEN] = SERVER_IP;
		int sock = 0;
		int valread;
		socklen_t recvlen, addrLocLen, addrExtLen;
		struct sockaddr_in address_local, address_external;
		bool InitHandshake = false;

		/**
		 * Transfer sizes
		 */
		int16_t bufferSize = DEFAULT_BUFFSIZE;
		int16_t packetSize = PACKETSIZE;
	};
	#endif
}



#endif