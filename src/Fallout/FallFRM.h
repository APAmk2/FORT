#pragma once

#include "../Utils/ByteReader.hpp"
#include "../Utils/BaseDataTypes.h"
#include <vector>
#include <string>

class FrmFrame_t
{
public:
	uint16_t Width = 1;
	uint16_t Height = 1;
	uint32_t FrameSize = 1;
	int16_t XShift = 0;
	int16_t YShift = 0;
	std::vector<uint8_t> FrameData;

	FrmFrame_t(ByteReader* reader);
};

class FallFrm_t
{
public:
	//Header
	uint32_t Version = 4;
	uint16_t Fps = 8;
	uint16_t ActionFrame = 0;
	uint16_t FramesPerDir = 1;
	std::vector<int16_t> XShift;
	std::vector<int16_t> YShift;
	std::vector<uint32_t> DataShift;
	uint32_t DataSize = 1;
	//Data
	std::vector<FrmFrame_t*> Frames;

	uint8_t DirCount = 6;

	FallFrm_t(ByteReader* reader);
};