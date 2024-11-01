#pragma once

#include "../Utils/MemoryReader.hpp"
#include "../Utils/BaseDataTypes.h"
#include <vector>
#include <string>

#define MSK_WIDTH 352
#define MSK_HEIGHT 300

class FallMSK_t
{
public:
	std::vector<uint8_t> PixelMask;

	std::string Filename = "";

	FallMSK_t(MemoryReader* reader);
};