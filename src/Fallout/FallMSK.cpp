#include "FallMSK.h"

FallMSK_t::FallMSK_t(MemoryReader* reader)
{
	PixelMask.resize(13200);
	reader->Read(&PixelMask[0], 13200);
}