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
	void drawWindow();
	void initWindow();
private:
	std::string fallMapfilename = "test.fotxtb";
	FallMap_t* fallMapFile = nullptr;
};