#pragma once

#include "SDL.h"
#include "../Utils/BaseToolWindow.h"
#include "FallMSK.h"
#include <vector>
#include <string>

class FallMSKWindow : public BaseToolWindow
{
public:
	FallMSKWindow(bool visibility) : BaseToolWindow(visibility) { }
	void DrawWin();
	void InitWin();
	void ProcessMenuBtn();
private:
	std::string Filename = "test.msk";
	FallMSK_t* File = nullptr;
	SDL_Texture* Tex = nullptr;
};