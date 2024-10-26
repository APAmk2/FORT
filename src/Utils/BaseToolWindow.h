#pragma once

class BaseToolWindow
{
public:
	BaseToolWindow() {};
	BaseToolWindow(bool visibility) { this->Visible = visibility; };

	virtual void DrawWin() {};
	virtual void InitWin() {};
	virtual void SetVisible(bool val) { this->Visible = val; };
	virtual bool GetVisible() { return this->Visible; };

	SDL_Renderer* Renderer = nullptr;
private:
	bool Visible = true;
};