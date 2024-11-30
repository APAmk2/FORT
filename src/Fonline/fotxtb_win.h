#pragma once

#include "SDL.h"
#include "../Utils/BaseWindow.h"
#include "fotxtb.h"
#include <vector>
#include <string>

class FoTxtbWindow : public BaseWindow
{
public:
	FoTxtbWindow(bool visibility) : BaseWindow(visibility) { }
	void DrawWin();
	void InitWin();
	void DestroyWin();
	void ProcessMenuBtn();
private:
	std::string Filename = "test.fotxtb";
	FoTxtb_t* File = nullptr;
};