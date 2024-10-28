#pragma once

#include "SDL.h"
#include "../Utils/BaseToolWindow.h"
#include "FallProto.h"
#include <vector>
#include <string>

class FallProtoWindow : public BaseToolWindow
{
public:
	FallProtoWindow(bool visibility) : BaseToolWindow(visibility) { }
	void DrawWin();
	void InitWin();
private:
	std::string Filename = "test.pro";
	FallProto_t* File = nullptr;
};