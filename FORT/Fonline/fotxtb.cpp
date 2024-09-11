#include "fotxtb.h"
#include <stdio.h>

using namespace std;

Fotxtb_t::Fotxtb_t(ByteReader* reader)
{
	entriesCount = reader->u32();
	
	for (uint32_t i = 0; i < entriesCount; i++)
	{
		uint32_t key = reader->u32();
		uint32_t str_size = reader->u32();

		string val = "";
		if (str_size > 0)
		{
			val = reader->string(str_size);
		}

		entries.emplace(key, val);
	}
}