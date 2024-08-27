#include "FTSprite.h"
#include "imgui.h"
#include "imgui_stdlib.h"

void FTSpriteWindow::drawWindow()
{
	if (!getVisible()) return;
	ImGui::Begin("Fallout:Tactics Sprite Tool");
	ImGui::End();
}
void FTSpriteWindow::initWindow()
{

}