#pragma once

#include "../Utils/MemoryReader.hpp"
#include "../Utils/BaseDataTypes.h"
#include <vector>
#include <string>

class FTZar_t
{
public:
	uint32_t Width = 1;
	uint32_t Height = 1;
	uint8_t Type = 0x34;
	uint8_t PalettePresent = 0;
	uint8_t DefColor = 0;
	uint32_t PaletteCount = 1;
	std::vector<ColorRGBA> Palette = { ColorRGBA(0, 0, 0, 0) };
	std::vector<ColorRGBA> Pixels;

	std::string Filename = "";

	FTZar_t(MemoryReader* reader, std::vector<ColorRGBA>* pal = nullptr);
};