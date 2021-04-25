#ifndef DATAPACKET_H
#define DATAPACKET_H

// int main(int argc, char const *argv[])
// {
// 	/* code */
// 	return 0;
// }


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


struct DataPacket {
	/**
	 * Data packet for user data
	 */
	struct DP {
		char16_t characters[DEFAULT_BUFFSIZE]{0};
		int16_t integers[DEFAULT_BUFFSIZE]{0};
		int8_t booleans[DEFAULT_BUFFSIZE]{0};
		_Float64 decimals[DEFAULT_BUFFSIZE]{0};
	};

	DP dp;

	/**
	 * Setters
	 */
	void setCharacters(int index, char value) { dp.characters[index] = (char16_t)value; }
	void setIntegers(int index, int value) { dp.integers[index] = (int16_t)value; }
	void setBooleans(int index, bool value) { dp.booleans[index] = (int8_t)value; }
	void setDecimals(int index, float value) { dp.decimals[index] = (_Float64)value; }

	/**
	 * Getters
	 */
	char getCharacters(int index) { return (char)dp.characters[index]; }
	int getIntegers(int index) { return (int)dp.integers[index]; }
	bool getBooleans(int index) { return (bool)dp.booleans[index]; }
	float getDecimals(int index) { return (float)dp.decimals[index]; }
};

#define PACKETSIZE sizeof(DataPacket::DP)

#endif