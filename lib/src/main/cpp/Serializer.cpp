#include "Serializer.h"

namespace UDP_TransferNT {
	template<typename t, typename v>
	t *Serializer::serialCycler(t *singleDt, v *data, bool reverse) {
		t *datatype = (t *)data;

		int i = 0;

		while (i < DEFAULT_BUFFSIZE) {
			if (reverse) {
				singleDt[i] = *datatype;
			} else {
				*datatype = singleDt[i];
			}
			datatype++;
			i++;
		}

		return datatype;
	}

	/**
	 * 
	 * 
	 * SERIALIZER/DESERIALIZER MUST PERFORM IN ORDER (characters, integers, booleans, decimals)
	 * 
	 * 
	 */
	void Serializer::serialize(DataPacket &dp, char *data) {
		auto *t1 = serialCycler(dp.dp.characters, data, false);
		auto *t2 = serialCycler(dp.dp.integers, t1, false);
		auto *t3 = serialCycler(dp.dp.booleans, t2, false);
		auto *t4 = serialCycler(dp.dp.decimals, t3, false);
	}

	void Serializer::serialize_h(DataPacket_H &dph, char *data) {
		auto *t1 = serialCycler(dph.dataTrue, data, false);
		auto *t2 = serialCycler(dph.data_buffersize, t1, false);
		auto *t3 = serialCycler(dph.data_packetsize, t2, false);
		auto *t4 = serialCycler(dph.portNum, t3, false);
		auto *t5 = serialCycler(dph.version, t4, false);
		auto *t6 = serialCycler(dph.complete, t5, false);
	}


	void Serializer::deserialize(DataPacket &dp, char *data) {
		auto *t1 = serialCycler(dp.dp.characters, data, true);
		auto *t2 = serialCycler(dp.dp.integers, t1, true);
		auto *t3 = serialCycler(dp.dp.booleans, t2, true);
		auto *t4 = serialCycler(dp.dp.decimals, t3, true);
	}

	void Serializer::deserialize_h(DataPacket_H &dph, char *data) {
		auto *t1 = serialCycler(dph.dataTrue, data, true);
		auto *t2 = serialCycler(dph.data_buffersize, t1, true);
		auto *t3 = serialCycler(dph.data_packetsize, t2, true);
		auto *t4 = serialCycler(dph.portNum, t3, true);
		auto *t5 = serialCycler(dph.version, t4, true);
		auto *t6 = serialCycler(dph.complete, t5, true);
	}
}
