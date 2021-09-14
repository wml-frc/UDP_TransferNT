#ifndef DATAPACKET_H
#define DATAPACKET_H

#include "nt_headers.h"

namespace UDP_TransferNT {
	/**
	 * Datapacket for user data
	 */
	struct DataPacket {

		struct DP_USER {
			char characters[DEFAULT_BUFFER_SIZE]{0};
			int integers[DEFAULT_BUFFER_SIZE]{0};
			bool booleans[DEFAULT_BUFFER_SIZE]{0};
			float decimals[DEFAULT_BUFFER_SIZE]{0};
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
}

#endif // DATAPACKET_H