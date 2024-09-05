#pragma once

#include "SDL.h"
#include "./Utils/ByteReader.hpp"
#include "./Utils/BaseToolWindow.h"
#include "./Utils/BaseDataTypes.h"
#include <vector>
#include <string>

class ZarImage_t
{
public:
	std::vector<ucolor> pixels;
	uint32_t width;
	uint32_t height;

	ZarImage_t(ByteReader* reader);
};

class AnimImgHdr_t
{
public:
	bool isPacked = false;
	uint32_t unpacked_size = 0;
	std::vector<uint8_t> packed_data;
	uint32_t pos = 0;

	AnimImgHdr_t(ByteReader* reader);
};

class AnimHdr_t
{
public:
	AnimImgHdr_t* animImgHdr = nullptr;
	std::string anim_name = "default";
	uint32_t frame_count = 0;
	uint32_t dirs = 0;
	std::vector<rect> bboxes;

	AnimHdr_t(ByteReader* reader);
};

class SequenceHdr_t
{
public:
	uint32_t items_count = 0;
	std::vector<int16_t> frame_num;
	std::string seq_name = "default";
	uint16_t anim_hdr_indx = 0;

	SequenceHdr_t(ByteReader* reader);
};

class FTSprite_t
{
public:
	uint32_t center_x = 0;
	uint32_t center_y = 0;
	uint32_t seq_count = 0;
	std::vector<SequenceHdr_t> seq_hdrs;
	uint32_t anim_count = 0;
	std::vector<AnimHdr_t> anim_hdrs;

	std::string filename = "test.png";

	FTSprite_t(ByteReader* reader);
};

class FTSpriteWindow : public BaseToolWindow
{
public:
	FTSpriteWindow(bool visibility) : BaseToolWindow(visibility) { }
	void drawWindow();
	void initWindow();
private:
	int FTSwidth = 0, FTSheight = 0, FTSDir = 0;
	FTSprite_t* FTSFile = nullptr;
	std::string FTSfilename = "test.png";
	uint32_t FTSFPSTimer = 0;
	SDL_Texture* FTSTex = nullptr;
};