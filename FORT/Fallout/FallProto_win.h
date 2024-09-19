#pragma once

#include "SDL.h"
#include "./Utils/BaseToolWindow.h"
#include "FallProto.h"
#include <vector>
#include <string>

class FallProtoWindow : public BaseToolWindow
{
public:
	FallProtoWindow(bool visibility) : BaseToolWindow(visibility) { }
	void drawWindow();
	void initWindow();
private:
	std::string fallProtofilename = "test.pro";
	FallProto_t* fallProtoFile = nullptr;
};