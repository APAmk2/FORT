#pragma once

#include "SDL.h"
#include "../Utils/BaseToolWindow.h"
#include "Fo2D.h"
#include <vector>
#include <string>

class Fo2DWindow : public BaseToolWindow
{
public:
	Fo2DWindow(bool visibility) : BaseToolWindow(visibility) { }
	void DrawWin();
	void InitWin();
	void ProcessMenuBtn();
private:
	uint16_t Width = 0, Height = 0;
	int16_t Dir = 0;
	std::string Filename = "test.png";
	Fo2D_t* File = nullptr;
	uint32_t FPSTimer = 0;
	SDL_Texture* Tex = nullptr;
};