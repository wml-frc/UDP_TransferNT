#include <iostream>
#include "UDP_TransferNT.h"

using namespace UDP_TransferNT;

int main() {
	std::cout << "Test Run..." << std::endl;

	UDP_TransferNT::Network network(Network::Type::SERVER, Network::ConnectionType::ANY);
	// network.init();
	return 0;
}