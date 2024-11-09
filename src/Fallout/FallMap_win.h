#pragma once

#include "SDL.h"
#include "../Utils/BaseToolWindow.h"
#include "FallMap.h"
#include <vector>
#include <string>

class FallMapWindow : public BaseToolWindow
{
public:
	FallMapWindow(bool visibility) : BaseToolWindow(visibility) { }
	void DrawWin();
	void InitWin();
	void ProcessMenuBtn();
private:
	std::string Filename = "test.map";
	FallMap_t* File = nullptr;
};