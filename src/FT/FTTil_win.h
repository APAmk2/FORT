#pragma once

#include "SDL.h"
#include "../Utils/BaseToolWindow.h"
#include "FTTil.h"
#include <vector>
#include <string>

class FTTilWindow : public BaseToolWindow
{
public:
	FTTilWindow(bool visibility) : BaseToolWindow(visibility) { }
	void DrawWin();
	void InitWin();
	void ProcessMenuBtn();
private:
	uint16_t Width = 0, Height = 0;
	uint32_t FPSTimer = 0;
	std::string Filename = "test.til";
	FTTil_t* File = nullptr;
	SDL_Texture* Tex = nullptr;
};