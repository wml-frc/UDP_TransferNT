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
		uint16_t getHandshakePort() { return this->_h_port; }

		void setPort(uint16_t port) { this->_port = port; }
		void setIP(const char ip[IP_LEN]) {
			for (int i = 0; i < IP_LEN; i++) {
				this->_ipaddress[i] = ip[i];
			}
		}

		uint16_t getPort() { return _port; }
		const char *getIP() { return _ipaddress; }
		int &getSocket() { return _sock; }
		int &getValread() { return _valread; }
		socklen_t &getRecvLen() { return _recvlen; }
		struct sockaddr_in &getLocalAddress() { return _address_local; }
		struct sockaddr_in &getExternalAddress() { return _address_local; }

		socklen_t &getLocalAddressLen() { 
			_addrLocLen = sizeof(_address_local);
			return _addrLocLen;
		}
		socklen_t &getExternalAddressLen() { 
			_addrExtLen = sizeof(_address_external);
			return _addrExtLen; 
		}

		int16_t &getBufferSize() { return _bufferSize; }
		int16_t &getPacketSize() { return _packetSize; }

	 private:

		/**
		 * Socket Values
		 */
		uint16_t _h_port = HANDSHAKE_PORT;
		uint16_t _port = PORT;
		char _ipaddress[IP_LEN] = SERVER_IP;
		int _sock = 0;
		int _valread;
		socklen_t _recvlen, _addrLocLen, _addrExtLen;
		struct sockaddr_in _address_local, _address_external;
		bool _InitHandshake = false;

		/**
		 * Transfer sizes
		 */
		int16_t _bufferSize = DEFAULT_BUFFSIZE;
		int16_t _packetSize = PACKETSIZE;
	};
	#endif
}



#endif