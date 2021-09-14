#include <iostream>
#include "UDP_TransferNT.h"

int main() {
	std::cout << "Test Run..." << std::endl;

	UDP_TransferNT::Network network(UDP_TransferNT::Network::Type::SERVER);
	network.init();
	return 0;
}