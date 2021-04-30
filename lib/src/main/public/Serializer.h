#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "nt_headers.h"
#include "DataPacket.h"

namespace UDP_TransferNT {
	class Serializer {
	 public:

		/**
		 * Serialize data into one long character array to be sent over
		 */
		static void serialize(DataPacket *dp, char *data);
		static void serialize_h(DataPacket_H *dp, char *data);

		/**
		 * Deserialize data from one long character array to datapacket
		 */
		static void deserialize(DataPacket *dp, char *data);
		static void deserialize_h(DataPacket_H *dp, char *data);

		
		/**
		 * Templated cycler
		 */
		template<typename t, typename v>
		static t *serialCycler(t *singleDt, v *data, bool reverse = false);
	};
}

#endif