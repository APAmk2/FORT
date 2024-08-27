#pragma once

#include "./Utils/ByteReader.hpp"
#include "./Utils/BaseToolWindow.h"
#include "./Utils/BaseDataTypes.h"
#include "SDL_opengl.h"
#include <vector>
#include <string>

class FTSprite_t
{

};

class FTSpriteWindow : public BaseToolWindow
{
public:
	FTSpriteWindow(bool visibility) : BaseToolWindow(visibility) { }
	void drawWindow();
	void initWindow();
private:
	int FTSwidth = 0, FTSDheight = 0, FTSDir = 0;
	std::string FTSfilename = "test.png";
	FTSprite_t* FTSFile = nullptr;
	uint32_t FTSFPSTimer = 0;
	GLuint FTSTex = 0;
};