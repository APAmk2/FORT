#pragma once

#include "SDL.h"
#include "../Utils/BaseWindow.h"
#include "Fo2D.h"
#include <vector>
#include <string>

class Fo2DWindow : public BaseWindow
{
public:
	Fo2DWindow(bool visibility) : BaseWindow(visibility) { }
	void DrawWin();
	void InitWin();
	void DestroyWin();
	void ProcessMenuBtn();
private:
	uint16_t Width = 0, Height = 0;
	int16_t Dir = 0;
	std::string Filename = "test.png";
	Fo2D_t* File = nullptr;
	uint32_t FPSTimer = 0;
	SDL_Texture* Tex = nullptr;
};