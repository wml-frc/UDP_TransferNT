#ifndef DATAPACKET_H
#define DATAPACKET_H

#include "nt_headers.h"

/**
 * Data packet for handshake
 */
struct DataPacket_H {
	// Simple check for data
	int16_t dataTrue = false;
	int16_t data_buffersize = 0;
	char version[sizeof(NETWORK_VERSION)] = NETWORK_VERSION;
};

#define PACKETSIZE_H sizeof(DataPacket_H)


/**
 * Data packet for user data
 */
struct DataPacket {
	char16_t characters[DEFAULT_BUFFSIZE]{0};
	int16_t integers[DEFAULT_BUFFSIZE]{0};
	int8_t booleans[DEFAULT_BUFFSIZE]{0};
	_Float64 decimals[DEFAULT_BUFFSIZE]{0};
};

#define PACKETSIZE sizeof(DataPacket)

#endif