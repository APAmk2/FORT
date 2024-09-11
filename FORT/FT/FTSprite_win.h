#pragma once

#include "SDL.h"
#include "FTSprite.h"
#include "./Utils/ByteReader.hpp"
#include "./Utils/BaseToolWindow.h"
#include <vector>
#include <string>

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