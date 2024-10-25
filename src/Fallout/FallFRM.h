#pragma once

#include "../Utils/ByteReader.hpp"
#include "../Utils/BaseDataTypes.h"
#include <vector>
#include <string>

class FrmFrame_t
{
public:
	uint16_t width = 1;
	uint16_t height = 1;
	uint32_t frameSize = 1;
	int16_t xShift = 0;
	int16_t yShift = 0;
	std::vector<uint8_t> frameData;

	FrmFrame_t(ByteReader* reader);
};

class FallFrm_t
{
public:
	//Header
	uint32_t version = 4;
	uint16_t fps = 8;
	uint16_t actionFrame = 0;
	uint16_t framesPerDir = 1;
	std::vector<int16_t> xShift;
	std::vector<int16_t> yShift;
	std::vector<uint32_t> dataShift;
	uint32_t dataSize = 1;
	//Data
	std::vector<FrmFrame_t*> frames;

	uint8_t dirs = 6;

	FallFrm_t(ByteReader* reader);
};