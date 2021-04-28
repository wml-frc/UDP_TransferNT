#include "Handshake.h"

void Handshake::handShake(Socket *userSock, bool server, bool any_con, int timeoutUS) {

	/**
	 * Create temporary handshake socket
	 */
	Socket sock;
	// sock = new Socket();

	/**
	 * Structured Timeout
	 */
	struct timeval tv;
	tv.tv_sec = 0;
	tv.tv_usec = timeoutUS;


	/**
	 * Set IP
	 */
	struct hostent *host;
	host = (struct hostent *)gethostbyname((char *)"127.0.0.1");

	/**
	 * Create handshake socket
	 */
	if ((*sock.getSocket() = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
		KILL("HANDSHAKE SOCKET ERROR");
	}

	/**
	 * Memset
	 */
	if (any_con) {
		std::cout << "Any connection zeroing" << std::endl;
		memset(sock.getLocalAddress(), 0, *sock.getLocalAddressLen());
		memset(sock.getExternalAddress(), 0, *sock.getExternalAddressLen());
	}

	/**
	 * Set the address as handshake address
	 */
	if (server) {
		std::cout << "Setting server handshake address" << std::endl;
		sock.getLocalAddress()->sin_family = AF_INET;
		sock.getLocalAddress()->sin_port = htons(HANDSHAKE_PORT);
		sock.getLocalAddress()->sin_addr.s_addr = htonl(INADDR_ANY);
	} else {
		std::cout << "Setting client handshake address" << std::endl;
		sock.getExternalAddress()->sin_family = AF_INET;
		sock.getExternalAddress()->sin_port = htons(HANDSHAKE_PORT);
		if (any_con) {
			sock.getExternalAddress()->sin_addr.s_addr = INADDR_ANY;
		} else {
			sock.getExternalAddress()->sin_addr = *((struct in_addr *)host->h_addr);
		}
	}

	/**
	 * clear address
	 */
	if (!any_con) {
		std::cout << "IP Specific zeroing" << std::endl;
		bzero(sock.getLocalAddress()->sin_zero, 8);
		bzero(sock.getExternalAddress()->sin_zero, 8);
	}

	/**
	 * Bind to the handshake socket
	 */
	if (server) {
		if (bind(*sock.getSocket(), (struct sockaddr *)sock.getLocalAddress(), *sock.getLocalAddressLen()) < 0) {
			KILL("HANDSHAKE BIND ERROR");
		}
	} else {
		if (!any_con) {
			std::cout << "Client Connecting" << std::endl;
			if (connect(*sock.getSocket(), (struct sockaddr *)sock.getExternalAddress(), *sock.getExternalAddressLen()) < 0) {
				KILL("HANDSHAKE CONNECT ERROR");
			}
		}
	}


	/**
	 * Set socket timeout
	 */
	// if (setsockopt(sock.getSocket(), SOL_SOCKET, SO_RCVTIMEO, &tv, sizeof(tv)) < 0) {
	// 	KILL("HANDSHAKE TIMEOUT");
	// }


	std::cout << "Finished Binding: Sending and receiving handshake" << std::endl;

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
	dph.data_buffersize[0] = *sock.getBufferSize();
	dph.data_packetsize[0] = *sock.getPacketSize();
	dph.portNum[0] = *sock.getPort();
	dph.complete[0] = -1;

	if (server) {

		while (dph.complete[0] != 1) {
			sleep(5);
			std::cout << "Server sending handshake data" << std::endl;
			char buffer[PACKETSIZE_H];
			dph.dataTrue[0] = 1;

			Serializer::serialize_h(&dph, buffer);

			if (sendto(*sock.getSocket(), buffer, sizeof(buffer), 0, (const struct sockaddr *)sock.getExternalAddress(), *sock.getExternalAddressLen()) < 0) {
				ERROR_PRINT("HANDSHAKE SEND SERVER");
			}

			sleep(5);
			std::cout << "Server receiving handshake data" << std::endl;
			*sock.getValread() = recvfrom(*sock.getSocket(), buffer, sizeof(buffer), 0, (struct sockaddr *)sock.getExternalAddress(), sock.getExternalAddressLen());
			if (sock.getValread() < 0) {
				ERROR_PRINT("HANDSHAKE RECV SERVER");
			}

			Serializer::deserialize_h(&dph, buffer);
		}

		close(*sock.getSocket()); // Close socket
	} else {
		while (dph.dataTrue[0] != 1) {
			std::cout << "Client receiving handshake data" << std::endl;
			char buffer[PACKETSIZE_H];

			if (any_con) {
				std::cout << "Any con recv" << std::endl;
				*sock.getValread() = recvfrom(*sock.getSocket(), buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)sock.getExternalAddress(), sock.getExternalAddressLen());
			} else {
				std::cout << "IP Spec recv" << std::endl;
				*sock.getValread() = recvfrom(*sock.getSocket(), buffer, sizeof(buffer), 0, (struct sockaddr *)NULL, NULL);
				// sock.getValread() = recvfrom(sock.getSocket(), buffer, sizeof(buffer), MSG_WAITALL, (struct sockaddr *)sock.getExternalAddress(), sock.getExternalAddressLen());
			}

			std::cout << "Data received" << std::endl;

			if (sock.getValread() < 0) {
				ERROR_PRINT("HANDSHAKE CLIENT RECV");
			} 

			/**
			 * Deserialize and read data
			 */
			Serializer::deserialize_h(&dph, buffer);

			std::cout << "Server Buffer size: " << dph.data_buffersize[0] << std::endl;
			std::cout << "Server Packet size: " << dph.data_packetsize[0] << std::endl;
			std::cout << "Server Port: " << dph.portNum[0] << std::endl;
			std::cout << "Server Network Version: " << dph.version[0] << std::endl;

			/**
			 * Set client socket to match server
			 */
			*userSock->getBufferSize() = dph.data_buffersize[0];
			*userSock->getPacketSize() = dph.data_packetsize[0];
			userSock->setPort(dph.portNum[0]);

			dph.complete[0] = 1;

			sleep(5);
			std::cout << "Client sending handshake data" << std::endl;

			/**
			 * Serialize and tell server handshake complete
			 */
			Serializer::serialize_h(&dph, buffer);

			if (any_con) {
				if (sendto(*sock.getSocket(), buffer, sizeof(buffer), 0, (const struct sockaddr *)sock.getExternalAddress(), *sock.getExternalAddressLen()) < 0) {
					ERROR_PRINT("HANDSHAKE SEND CLIENT");
					// std::cout << "ERROR SEND: CLIENT" << std::endl;
				}
			}
		}

		close(*sock.getSocket()); // Close socket
	}
}