#pragma once

#include "SDL.h"
#include "../Utils/BaseWindow.h"
#include "FTZar.h"
#include <vector>
#include <string>

class FTZarWindow : public BaseWindow
{
public:
	FTZarWindow(bool visibility) : BaseWindow(visibility) { }
	void DrawWin();
	void InitWin();
	void DestroyWin();
	void ProcessMenuBtn();
private:
	uint16_t Width = 0, Height = 0;
	std::string Filename = "test.zar";
	FTZar_t* File = nullptr;
	SDL_Texture* Tex = nullptr;
};