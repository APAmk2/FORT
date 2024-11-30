#pragma once

#include "SDL.h"
#include "../Utils/BaseWindow.h"
#include "FallRIX.h"
#include <vector>
#include <string>

class FallRIXWindow : public BaseWindow
{
public:
	FallRIXWindow(bool visibility) : BaseWindow(visibility) { }
	void DrawWin();
	void InitWin();
	void DestroyWin();
	void ProcessMenuBtn();
private:
	uint16_t Width = 0, Height = 0;
	std::string Filename = "test.rix";
	FallRIX_t* File = nullptr;
	SDL_Texture* Tex = nullptr;
};