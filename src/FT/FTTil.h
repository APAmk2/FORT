#pragma once

#include "../Utils/MemoryReader.hpp"
#include "../Utils/BaseDataTypes.h"
#include "FTZar.h"
#include <vector>
#include <string>

class FTTil_t
{
public:
	uint32_t Width = 1;
	uint32_t Height = 1;
	uint32_t FrameCount = 1;
	std::vector<FTZar_t*> ZarFrames;

	std::string Filename = "";

	FTTil_t(MemoryReader* reader);
};