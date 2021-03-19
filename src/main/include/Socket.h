#ifndef SOCKET_H
#define SOCKET_H
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
	int *getRecvLen() { return &recvlen; }
	struct sockaddr_in *getLocalAddress() { return &address_local; }
	struct sockaddr_in *getExternalAddress() { return &address_local; }
	socklen_t *getExternalAddressLen() { return &addressExt_len; }

 private:
	uint16_t h_port = HANDSHAKE_PORT;
	uint16_t port = PORT;
	char ipaddress[20] = SERVER_IP;
	int sock = 0;
	int valread;
	int recvlen;
	struct sockaddr_in address_local, address_external;
	socklen_t addressExt_len = sizeof(address_external);
	bool InitHandshake = false;
};


#endif