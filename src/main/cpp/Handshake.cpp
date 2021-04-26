#include "Handshake.h"

void Handshake::handShake(Socket *sock, Socket *userSock, bool server, bool any_con) {
	/**
	 * Set IP
	 */
	struct hostent *host;
	host = (struct hostent *)gethostbyname((char *)sock->getIP());

	/**
	 * Create handshake socket
	 */
	if ((*sock->getSocket() == socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		KILL("HANDSHAKE SOCKET ERROR");
	}

	/**
	 * Memset
	 */
	if (any_con) {
		memset(sock->getLocalAddress(), 0, *sock->getLocalAddressLen());
		memset(sock->getExternalAddress(), 0, *sock->getExternalAddressLen());
	}

	/**
	 * Set the address as handshake address
	 */
	if (server) {
		sock->getLocalAddress()->sin_family = AF_INET;
		sock->getLocalAddress()->sin_port = htons(HANDSHAKE_PORT);
		sock->getLocalAddress()->sin_addr.s_addr = htonl(INADDR_ANY);
	} else {
		sock->getExternalAddress()->sin_family = AF_INET;
		sock->getExternalAddress()->sin_port = htons(HANDSHAKE_PORT);
		if (any_con) {
			sock->getExternalAddress()->sin_addr.s_addr = INADDR_ANY;
		} else {
			sock->getExternalAddress()->sin_addr = *((struct in_addr *)host->h_addr);
		}
	}

	/**
	 * clear address
	 */
	if (!any_con) {
		bzero(sock->getLocalAddress()->sin_zero, 8);
		bzero(sock->getExternalAddress()->sin_zero, 8);
	}

	/**
	 * Bind to the handshake socket
	 */
	if (server) {
		if (bind(*sock->getSocket(), (struct sockaddr *)sock->getLocalAddress(), *sock->getLocalAddressLen()) < 0) {
			KILL("HANDSHAKE BIND ERROR");
		}
	} else {
		if (!any_con) {
			if (connect(*sock->getSocket(), (struct sockaddr *)sock->getExternalAddress(), *sock->getExternalAddressLen()) < 0) {
				KILL("HANDSHAKE CONNECT ERROR");
			}
		}
	}

	/**
	 * 
	 * 
	 * 
	 * If server send server sizing data (else receive the new sizing)
	 * 
	 * 
	 * 
	 */

	/**
	 * Set datapacket to local
	 */
	DataPacket_H dph;
	dph.dataTrue[0] = 0;
	dph.data_buffersize[0] = *sock->getBufferSize();
	dph.data_packetsize[0] = *sock->getPacketSize();
	dph.portNum[0] = *sock->getPort();
	dph.complete[0] = -1;

	if (server) {

		while (dph.complete[0] != 1) {
			char buffer[PACKETSIZE_H];
			dph.dataTrue[0] = 1;

			Serializer::serialize_h(&dph, buffer);

			if (sendto(*sock->getSocket(), buffer, sizeof(buffer), 0, (const struct sockaddr *)NULL, *sock->getExternalAddressLen()) < 0) {
				ERROR_PRINT("HANDSHAKE SEND SERVER");
			}

			*sock->getValread() = recvfrom(*sock->getSocket(), buffer, sizeof(buffer), 0, (struct sockaddr *)sock->getExternalAddress(), sock->getExternalAddressLen());
			if (*sock->getValread() < 0) {
				ERROR_PRINT("HANDSHAKE RECV SERVER");
			}

			Serializer::deserialize_h(&dph, buffer);
		}

		close(*sock->getSocket()); // Close socket
	} else {

		while (dph.dataTrue[0] != 1) {
			char buffer[PACKETSIZE_H];

			if (any_con) {
				*sock->getValread() = recvfrom(*sock->getSocket(), buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)sock->getExternalAddress(), sock->getExternalAddressLen());
			} else {
				*sock->getValread() = recvfrom(*sock->getSocket(), buffer, sizeof(buffer), 0, (struct sockaddr *)NULL, NULL);
			}

			if (*sock->getValread() < 0) {
				ERROR_PRINT("HANDSHAKE CLIENT RECV");
			} 

			/**
			 * Deserialize and read data
			 */
			Serializer::deserialize_h(&dph, buffer);

			std::cout << "Server Buffer size: " << dph.data_buffersize << std::endl;
			std::cout << "Server Packet size: " << dph.data_packetsize << std::endl;
			std::cout << "Server Port: " << dph.portNum << std::endl;
			std::cout << "Server Network Version: " << dph.version << std::endl;

			/**
			 * Set client socket to match server
			 */
			*userSock->getBufferSize() = dph.data_buffersize[0];
			*userSock->getPacketSize() = dph.data_packetsize[0];
			*userSock->getPort() = dph.portNum[0];

			dph.complete[0] = 1;

			/**
			 * Serialize and tell server handshake complete
			 */
			Serializer::serialize_h(&dph, buffer);

			if (any_con) {
				if (sendto(*sock->getSocket(), buffer, sizeof(buffer), 0, (const struct sockaddr *)sock->getExternalAddress(), *sock->getExternalAddressLen()) < 0) {
					ERROR_PRINT("HANDSHAKE SEND CLIENT");
					// std::cout << "ERROR SEND: CLIENT" << std::endl;
				}
			}
		}

		close(*sock->getSocket()); // Close socket
	}
}