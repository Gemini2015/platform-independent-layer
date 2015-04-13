#pragma once

#include "PILPrerequisites.h"
#include "PILWindow.h"

class CustomWindow : public PIL::IWindowEventListener
{
public:
	CustomWindow(PIL::Window *w);

	~CustomWindow();

	void SetHidden(bool hidden);

	void SetPos(int32 left, int32 top);

	void SetSize(uint32 width, uint32 height);

	static CustomWindow* Create(std::string name);



	virtual void OnDestroy(const PIL::Window* w) override;

	virtual void OnSetActive(const PIL::Window* w, bool active) override;

	virtual void OnWindowMove(const PIL::Window* w, const PIL::Point& oldPos, const PIL::Point& newPos) override;

	virtual void OnWindowResize(const PIL::Window* w, const PIL::Size& oldSize, const PIL::Size& newSize) override;

private:
	PIL::Window* mWindow;

};

class WindowListener : public PIL::IWindowEventListener
{


	virtual void OnCreate(const PIL::Window* w) override;

	virtual void OnSetActive(const PIL::Window* w, bool active) override;

	virtual void OnWindowMove(const PIL::Window* w, const PIL::Point& oldPos, const PIL::Point& newPos) override;

	virtual void OnWindowResize(const PIL::Window* w, const PIL::Size& oldSize, const PIL::Size& newSize) override;

	virtual bool OnClosing(const PIL::Window* w) override;

};