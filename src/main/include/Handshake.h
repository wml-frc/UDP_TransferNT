#ifndef HANDSHAKE_H
#define HANDSHAKE_H

#include "Socket.h"
#include "Serializer.h"
#include "nt_headers.h"

class Handshake {
 public:
	static void handShake(Socket *sock, Socket *userSock, bool server = false, bool any_con = false);
};

#endif