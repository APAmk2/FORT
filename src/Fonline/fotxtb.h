#pragma once

#include "../Utils/ByteReader.hpp"
#include "../Utils/BaseDataTypes.h"
#include <vector>
#include <string>
#include <map>

class FoTxtb_t
{
public:
	uint32_t EntriesCount = 0;
	std::map<uint32_t, std::string> Entries;

	std::string Filename = "";

	FoTxtb_t(ByteReader* reader);
};