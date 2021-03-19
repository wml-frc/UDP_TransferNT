#include "Serializer.h"

template<typename t>
void Serializer::serialCycler(t *singleDt, char *data, bool reverse) {
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
}

void Serializer::serialize(DataPacket *dp, char *data) {
	serialCycler(dp->characters, data, false); // Serialize characters
}

void Serializer::deserialize(DataPacket *dp, char *data) {
	serialCycler(dp->characters, data, true); // Deserialize characters
}