#include "FallShared.h"

PID_t::PID_t(ByteReader* reader)
{
	PIDType = reader->u8();
	reader->u8();
	PIDNum = reader->u16();
}