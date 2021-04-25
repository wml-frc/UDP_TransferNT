#ifndef SERVER_H
#define SERVER_H

#include "nt_headers.h"
#include "NetworkContainer.h"

class Server : public Network {
 public:
	Server() : Network(Type::SERVER, false) {
		std::cout << "UDP Server Created" << std::endl;
	}
 
 private:
	Type type;
};

#endif