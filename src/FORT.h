// FORT.h : Include file for standard system include files,
// or project specific include files.

#pragma once
#include "SDL.h"
#include "imgui.h"
#include "imgui_stdlib.h"
#include "Utils/BaseWindow.h"
#include <string>

struct ProgramSettings
{
	bool callExit = false;

#ifdef NEW_FONLINE
	std::string newFonlinePath = "";
#endif // NEW_FONLINE

#ifdef FALLOUT
	std::string falloutPath = "";
#endif // FALLOUT

#ifdef FALLOUT_TACTICS
	std::string falloutTacticsPath = "";
#endif // FALLOUT_TACTICS

};

extern ProgramSettings progSettings;
extern SDL_Renderer* Renderer;