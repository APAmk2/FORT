#pragma once

#include "./Utils/ByteReader.hpp"
#include "./Utils/BaseToolWindow.h"
#include "./Utils/BaseDataTypes.h"
#include "SDL_opengl.h"
#include <vector>
#include <string>

class frame_t
{
public:
	bool is_shared = false;
	uint16_t width = 0;
	uint16_t height = 0;
	int16_t next_x = 0;
	int16_t next_y = 0;
	std::vector<ucolor> pixels;
	uint16_t shared_indx = 0;

	frame_t(ByteReader* reader);
	frame_t() {};
};

class hdr_t
{
public:
	uint16_t frames_count = 0;
	uint16_t anim_ticks = 0;
	uint8_t dirs = 0;

	hdr_t(ByteReader* reader);
	hdr_t() {};
};

class data_t
{
public:
	hdr_t* hdr_ptr = nullptr;
	int16_t offs_x = 0;
	int16_t offs_y = 0;
	std::vector<frame_t> frames;

	data_t(ByteReader* reader, hdr_t* hdr_ptr);
	data_t() {};
};

class Fo2D_t
{
public:
	hdr_t* hdr = nullptr;
	std::vector<data_t> data;
	std::string filename = "";

	Fo2D_t(ByteReader* reader);
};

class Fo2DWindow : public BaseToolWindow
{
public:
	Fo2DWindow(bool visibility) : BaseToolWindow(visibility) { }
	void drawWindow();
	void initWindow();
private:
	int Fo2Dwidth = 0, Fo2Dheight = 0, Fo2DDir = 0;
	std::string Fo2Dfilename = "test.png";
	Fo2D_t* Fo2DFile = nullptr;
	uint32_t Fo2DFPSTimer = 0;
	GLuint Fo2DTex = 0;
};