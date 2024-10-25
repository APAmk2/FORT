#pragma once

#include "../Utils/ByteReader.hpp"
#include "../Utils/BaseDataTypes.h"
#include <vector>
#include <string>
#include <map>

class Fotxtb_t
{
public:
	std::string filename = "";
	uint32_t entriesCount = 0;
	std::map<uint32_t, std::string> entries;

	Fotxtb_t(ByteReader* reader);
};