#pragma once

#include "SDL.h"
#include "./Utils/BaseToolWindow.h"
#include "Fo2D.h"
#include <vector>
#include <string>

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
	SDL_Texture* Fo2DTex = nullptr;
};