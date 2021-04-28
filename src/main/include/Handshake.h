#ifndef HANDSHAKE_H
#define HANDSHAKE_H

#include "Socket.h"
#include "Serializer.h"
#include "nt_headers.h"

class Handshake {
 public:
	void handShake(Socket *userSock, bool server = false, bool any_con = false, int timeoutMicroSeconds = 100000);
 private:
};

#endif