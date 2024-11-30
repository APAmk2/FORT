#pragma once

#include "SDL.h"
#include "../Utils/BaseWindow.h"
#include "FallProto.h"
#include <vector>
#include <string>

class FallProtoWindow : public BaseWindow
{
public:
	FallProtoWindow(bool visibility) : BaseWindow(visibility) { }
	void DrawWin();
	void InitWin();
	void DestroyWin();
	void ProcessMenuBtn();
private:
	std::string Filename = "test.pro";
	FallProto_t* File = nullptr;
};