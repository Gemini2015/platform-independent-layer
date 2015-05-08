#include "CustomWindow.h"
#include "PILInclude.h"

#include <iostream>

CustomWindow::CustomWindow(std::string name)
	: mName(name)
	, mWindow(NULL)
{
	PIL::Root::SingletonPtr()->GetWindowManger()->AddListener(this);
}

CustomWindow::~CustomWindow()
{
	ReleaseWindow();
	PIL::Root::Singleton().GetWindowManger()->RemoveListener(this);
}

std::string CustomWindow::GetName() const
{
	return mName;
}

void CustomWindow::SetHidden(bool hidden)
{
	if (mWindow)
	{
		mWindow->SetHidden(hidden);
	}
}

void CustomWindow::SetPos(int32 left, int32 top)
{
	if (mWindow)
	{
		mWindow->SetWindowPosition(left, top);
	}
}

void CustomWindow::SetSize(uint32 width, uint32 height)
{
	if (mWindow)
	{
		mWindow->SetWindowSize(width, height);
	}
}

HRESULT CustomWindow::Create()
{
	if (mWindow != nullptr)
		return  E_FAIL;
	PIL::WindowManager* wm = PIL::Root::SingletonPtr()->GetWindowManger();
	PIL::Window* w = NULL;
	HRESULT hr = wm->NewWindow(mName, 100, 100, 320, 240, NULL, this, &w);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	mWindow = w;
	w->SetHidden(true);
	return S_OK;
}

void CustomWindow::OnDestroy(const PIL::Window* w)
{
	if (w == mWindow)
	{
		std::cout << w->GetWindowTitle() << " Destroy" << std::endl;
		mWindow = nullptr;
	}
}

void CustomWindow::OnSetActive(const PIL::Window* w, bool active)
{
	if (w == mWindow)
		std::cout << w->GetWindowTitle() << " " << (active ? "Active" : "Inactive") << std::endl;
}

void CustomWindow::OnWindowMove(const PIL::Window* w)
{
	if (mWindow == w)
		std::cout << w->GetWindowTitle() << " Move to (" << w->GetWindowPosition().x << "," << w->GetWindowPosition().y << ")" << std::endl;
}

void CustomWindow::OnWindowResize(const PIL::Window* w)
{
	if (mWindow == w)
		std::cout << w->GetWindowTitle() << " Resize to (" << w->GetWindowSize().width << "," << w->GetWindowSize().height << ")" << std::endl;
}

void CustomWindow::ReleaseWindow()
{
	if (mWindow == nullptr)
		return;
	HRESULT hr = mWindow->GetWindowManager()->ShutDownWindow(mWindow);
	if (!FAILED(hr))
	{
		std::cout << "Release Window " << GetName() << std::endl;
	}
}

void CustomWindow::ResetWindowPtr()
{
	mWindow = nullptr;
}

void CustomWindow::SwapBuffers()
{
	if (mWindow) mWindow->SwapBuffers(false);
}

PIL::Size CustomWindow::GetSize()
{
	if (mWindow) return mWindow->GetWindowSize();
	return PIL::Size(0, 0);
}
