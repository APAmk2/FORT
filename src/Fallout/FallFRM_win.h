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
	void drawWindow();
	void initWindow();
private:
	int FRMwidth = 0, FRMheight = 0, FRMDir = 0;
	std::string FRMfilename = "test.frm";
	FallFrm_t* FRMFile = nullptr;
	uint32_t FRMFPSTimer = 0;
	SDL_Texture* FRMTex = nullptr;
};