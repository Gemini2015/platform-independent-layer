#pragma once

#include "PILPrerequisites.h"
#include "PILWindow.h"

class CustomWindow : 
	public PIL::WindowObject,
	public PIL::IWindowEventListener
{
public:
	CustomWindow(std::string name);

	~CustomWindow();

	std::string GetName() const;

	void SetHidden(bool hidden);

	void SetPos(int32 left, int32 top);

	void SetSize(uint32 width, uint32 height);

	HRESULT Create();

	void ReleaseWindow();

	void ResetWindowPtr();

	virtual void OnDestroy(const PIL::Window* w) override;

	virtual void OnSetActive(const PIL::Window* w, bool active) override;

	virtual void OnWindowMove(const PIL::Window* w) override;

	virtual void OnWindowResize(const PIL::Window* w) override;

private:
	PIL::Window* mWindow;
	std::string mName;
};

class WindowListener : public PIL::IWindowEventListener
{
public:
	WindowListener() {}
	~WindowListener();

private:
	virtual void OnCreate(const PIL::Window* w) override;

	virtual void OnSetActive(const PIL::Window* w, bool active) override;

	virtual void OnWindowMove(const PIL::Window* w) override;

	virtual void OnWindowResize(const PIL::Window* w) override;

	virtual bool OnClosing(const PIL::Window* w) override;

	virtual void OnDestroy(const PIL::Window* w) override;

};