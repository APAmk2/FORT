#pragma once

#include "SDL.h"
#include "../Utils/BaseToolWindow.h"
#include "FallRIX.h"
#include <vector>
#include <string>

class FallRIXWindow : public BaseToolWindow
{
public:
	FallRIXWindow(bool visibility) : BaseToolWindow(visibility) { }
	void DrawWin();
	void InitWin();
	void ProcessMenuBtn();
private:
	uint16_t Width = 0, Height = 0;
	std::string Filename = "test.rix";
	FallRIX_t* File = nullptr;
	SDL_Texture* Tex = nullptr;
};