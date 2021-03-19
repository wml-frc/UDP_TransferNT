#ifndef SERIALIZER_H
#define SERIALIZER_H

#include "nt_headers.h"
#include "DataPacket.h"

class Serializer {
 protected:

	/**
	 * Serialize data into one long character array to be sent over
	 */
	static void serialize(DataPacket *dp, char *data);
	static void serialize_h(DataPacket *dp, char *data);

	/**
	 * Deserialize data from one long character array to datapacket
	 */
	static void deserialize(DataPacket *dp, char *data);
	static void deserialize_h(DataPacket *dp, char *data);

 private:
	
	/**
	 * Templated cycler
	 */
	template<typename t>
	static void serialCycler(t *singleDt, char *data, bool reverse = false);
};

#endif