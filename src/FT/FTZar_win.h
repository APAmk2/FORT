#pragma once

#include "SDL.h"
#include "../Utils/BaseToolWindow.h"
#include "FTZar.h"
#include <vector>
#include <string>

class FTZarWindow : public BaseToolWindow
{
public:
	FTZarWindow(bool visibility) : BaseToolWindow(visibility) { }
	void DrawWin();
	void InitWin();
private:
	uint16_t Width = 0, Height = 0;
	std::string Filename = "test.zar";
	FTZar_t* File = nullptr;
	SDL_Texture* Tex = nullptr;
};