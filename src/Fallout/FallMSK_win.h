#pragma once

#include "SDL.h"
#include "../Utils/BaseWindow.h"
#include "FallMSK.h"
#include <vector>
#include <string>

class FallMSKWindow : public BaseWindow
{
public:
	FallMSKWindow(bool visibility) : BaseWindow(visibility) { }
	void DrawWin();
	void InitWin();
	void DestroyWin();
	void ProcessMenuBtn();
private:
	std::string Filename = "test.msk";
	FallMSK_t* File = nullptr;
	SDL_Texture* Tex = nullptr;
};