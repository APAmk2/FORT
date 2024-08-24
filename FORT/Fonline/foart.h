#pragma once

#include "ByteReader.hpp"
#include <vector>
#include <filesystem>

struct ucolor
{
	uint8_t r;
	uint8_t g;
	uint8_t b;
	uint8_t a;
};

class frame_t
{
public:
	bool is_shared;
	uint16_t width;
	uint16_t height;
	int16_t next_x;
	int16_t next_y;
	std::vector<ucolor> pixels;
	uint16_t shared_indx;

	void read(ByteReader* reader);
};

class hdr_t
{
public:
	uint16_t frames_count;
	uint16_t anim_ticks;
	uint8_t dirs;

	void read(ByteReader* reader);
};

class data_t
{
public:
	hdr_t* hdr_ptr;
	int16_t offs_x;
	int16_t offs_y;
	std::vector<frame_t> frames;

	void read(ByteReader* reader);
};

class file_t
{
public:
	uint8_t check_num1;
	hdr_t hdr;
	std::vector<data_t> data;

	void read(ByteReader* reader);
};

class oldfile_t // Old file format, only for FO2 graphics support - APAMk2
{
public:
	uint32_t width;
	uint32_t height;
	std::vector<ucolor> pixels;

	void read(ByteReader* reader);
};

bool decodeFonline2D(std::filesystem::path& filename, int& width, int& height);
void decodeOldFonline2D(std::filesystem::path& filename, int& width, int& height);