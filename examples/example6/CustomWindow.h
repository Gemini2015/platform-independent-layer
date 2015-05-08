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

	PIL::Size GetSize();

	void SetSize(uint32 width, uint32 height);

	HRESULT Create();

	void ReleaseWindow();

	void ResetWindowPtr();

	virtual void OnDestroy(const PIL::Window* w) override;

	virtual void OnSetActive(const PIL::Window* w, bool active) override;

	virtual void OnWindowMove(const PIL::Window* w) override;

	virtual void OnWindowResize(const PIL::Window* w) override;

	void SwapBuffers();

private:
	PIL::Window* mWindow;
	std::string mName;
};