#pragma once

#include "SDL.h"
#include "../Utils/BaseWindow.h"
#include "FTTil.h"
#include <vector>
#include <string>

class FTTilWindow : public BaseWindow
{
public:
	FTTilWindow(bool visibility) : BaseWindow(visibility) { }
	void DrawWin();
	void InitWin();
	void DestroyWin();
	void ProcessMenuBtn();
private:
	uint16_t Width = 0, Height = 0;
	uint32_t FPSTimer = 0;
	std::string Filename = "test.til";
	FTTil_t* File = nullptr;
	SDL_Texture* Tex = nullptr;
};