#pragma once

class BaseToolWindow
{
public:
	BaseToolWindow() {};
	BaseToolWindow(bool visibility) { this->visible = visibility; };

	virtual void drawWindow() {};
	virtual void initWindow() {};
	virtual void setVisible(bool val) { this->visible = val; };
	virtual bool getVisible() { return this->visible; };
private:
	bool visible = true;
};