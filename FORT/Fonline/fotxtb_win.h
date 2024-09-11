#pragma once

#include "SDL.h"
#include "./Utils/BaseToolWindow.h"
#include "fotxtb.h"
#include <vector>
#include <string>

class FotxtbWindow : public BaseToolWindow
{
public:
	FotxtbWindow(bool visibility) : BaseToolWindow(visibility) { }
	void drawWindow();
	void initWindow();
private:
	std::string Fotxtbfilename = "test.fotxtb";
	Fotxtb_t* FotxtbFile = nullptr;
};