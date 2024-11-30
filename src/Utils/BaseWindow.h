#pragma once

class BaseWindow
{
public:
	BaseWindow() {};
	BaseWindow(bool visibility) { this->Visible = visibility; };

	virtual void DrawWin() {};
	virtual void InitWin() {};
	virtual void DestroyWin() {};
	virtual void ProcessMenuBtn() {};
	void SetVisible(bool val) 
	{
		this->Visible = val;
		OnVisibleChanged(val);
	};
	bool GetVisible() { return this->Visible; };
	virtual void OnVisibleChanged(bool val) {};
protected:
	bool Visible = true;
};