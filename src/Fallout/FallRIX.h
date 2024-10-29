#pragma once

#include "../Utils/MemoryReader.hpp"
#include "../Utils/BaseDataTypes.h"
#include <vector>
#include <string>

class FallRIX_t
{
public:
	uint16_t Width = 640;
	uint16_t Height = 480;
	uint8_t PalType = 0xAF;
	uint8_t StoreType = 0x00;
	std::vector<ColorRGB> Palette;
	std::vector<uint8_t> Pixels;

	std::string Filename = "";

	FallRIX_t(MemoryReader* reader);
};