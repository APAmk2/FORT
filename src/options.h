#pragma once

#include "SDL.h"
#include "Utils/BaseWindow.h"
#include <vector>
#include <string>

class OptionsWindow : public BaseWindow
{
public:
	OptionsWindow(bool visibility) : BaseWindow(visibility) { };
	void DrawWin();
	void InitWin();
	void ProcessMenuBtn();
};