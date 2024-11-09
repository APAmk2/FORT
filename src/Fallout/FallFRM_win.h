#pragma once

#include "SDL.h"
#include "../Utils/BaseToolWindow.h"
#include "FallFRM.h"
#include <vector>
#include <string>

class FallFRMWindow : public BaseToolWindow
{
public:
	FallFRMWindow(bool visibility) : BaseToolWindow(visibility) { }
	void DrawWin();
	void InitWin();
	void ProcessMenuBtn();
private:
	uint16_t Width = 0, Height = 0;
	int16_t Dir = 0;
	std::string Filename = "test.frm";
	FallFrm_t* File = nullptr;
	uint32_t FPSTimer = 0;
	SDL_Texture* Tex = nullptr;
};