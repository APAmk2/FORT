#include "../Utils/ByteReader.hpp"
#include <string>

// Also known as ProtoID, FID, or FRM ID
class PID_t
{
public:
	uint8_t PIDType = 0;
	uint16_t PIDNum = 0;

	PID_t(ByteReader* reader);
};