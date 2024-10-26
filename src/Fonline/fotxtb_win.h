#pragma once

#include "SDL.h"
#include "../Utils/BaseToolWindow.h"
#include "fotxtb.h"
#include <vector>
#include <string>

class FoTxtbWindow : public BaseToolWindow
{
public:
	FoTxtbWindow(bool visibility) : BaseToolWindow(visibility) { }
	void DrawWin();
	void InitWin();
private:
	std::string Filename = "test.fotxtb";
	FoTxtb_t* File = nullptr;
};