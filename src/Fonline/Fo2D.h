#pragma once

#include "../Utils/ByteReader.hpp"
#include "../Utils/BaseDataTypes.h"
#include <vector>
#include <string>

class Fo2DFrame_t
{
public:
	bool Shared = false;
	uint16_t Width = 0;
	uint16_t Height = 0;
	int16_t NextX = 0;
	int16_t NextY = 0;
	std::vector<ColorRGBA> Pixels;
	uint16_t SharedIndex = 0;

	Fo2DFrame_t(ByteReader* reader);
	Fo2DFrame_t() {};
};
class Fo2DData_t
{
public:
	int16_t XOffs = 0;
	int16_t YOffs = 0;
	std::vector<Fo2DFrame_t> Frames;

	Fo2DData_t(ByteReader* reader, uint16_t frameCount);
	Fo2DData_t() {};
};

class Fo2D_t
{
public:
	uint16_t FrameCount = 0;
	uint16_t AnimTicks = 0;
	uint8_t DirCount = 0;
	std::vector<Fo2DData_t> Data;
	std::string Filename = "";

	Fo2D_t(ByteReader* reader);
};