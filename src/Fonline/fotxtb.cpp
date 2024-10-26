#include "FoTxtb.h"

FoTxtb_t::FoTxtb_t(ByteReader* reader)
{
	EntriesCount = reader->u32();
	
	for (uint32_t i = 0; i < EntriesCount; i++)
	{
		uint32_t key = reader->u32();
		uint32_t strSize = reader->u32();

		std::string val = "";
		if (strSize)
		{
			val = reader->string(strSize);
		}

		Entries.emplace(key, val);
	}
}