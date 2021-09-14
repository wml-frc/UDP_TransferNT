#ifndef DATAPACKET_H
#define DATAPACKET_H

#include "nt_headers.h"

namespace UDP_TransferNT {
	/**
	 * Datapacket for user data
	 */
	struct DataPacket {

		struct DP_USER {
			char characters[(DATAPACKET_TYPESIZE)/sizeof(char)]{0};
			int integers[(DATAPACKET_TYPESIZE)/sizeof(int)]{0};
			bool booleans[(DATAPACKET_TYPESIZE)/sizeof(bool)]{0};
			float decimals[(DATAPACKET_TYPESIZE)/sizeof(float)]{0};
		};

		// DP_RAW dp_r;
		DP_USER dp;

		/**
		 * Setters
		 */
		void setCharacters(int index, char value) { dp.characters[index] = (char)value; }
		void setIntegers(int index, int value) { dp.integers[index] = (int)value; }
		void setBooleans(int index, bool value) { dp.booleans[index] = (int)value; }
		void setDecimals(int index, float value) { dp.decimals[index] = (float)value; }

		/**
		 * Getters
		 */
		char getCharacters(int index) { return (char)dp.characters[index]; }
		int getIntegers(int index) { return (int)dp.integers[index]; }
		bool getBooleans(int index) { return (bool)dp.booleans[index]; }
		float getDecimals(int index) { return (float)dp.decimals[index]; }
	};

	#define PACKETSIZE sizeof(DataPacket::DP_USER::characters) + sizeof(DataPacket::DP_USER::integers) + sizeof(DataPacket::DP_USER::booleans) + sizeof(DataPacket::DP_USER::booleans)  
}

#endif // DATAPACKET_H