#include "CustomWindow.h"
#include "PILInclude.h"

#include <iostream>

CustomWindow::CustomWindow(PIL::Window *w)
	:mWindow(w)
{
}

CustomWindow::~CustomWindow()
{
	mWindow = NULL;
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

CustomWindow* CustomWindow::Create(std::string name)
{
	PIL::WindowManager* wm = PIL::Root::SingletonPtr()->GetWindowManger();
	PIL::Window* w = NULL;
	HRESULT hr = wm->NewWindow(name, 100, 100, 320, 240, NULL, &w);
	if (FAILED(hr))
	{
		return NULL;
	}
	w->SetHidden(true);
	CustomWindow* window = new CustomWindow(w);
	if (window == NULL)
	{
		wm->DeleteWindow(w);
	}
	w->AddListener(window);
	return window;
}

void CustomWindow::OnDestroy(const PIL::Window* w)
{
	if (w)
	{
		std::cout << w->GetWindowTitle() << " Destroy" << std::endl;
	}
}

void CustomWindow::OnSetActive(const PIL::Window* w, bool active)
{
	std::cout << w->GetWindowTitle() << " " << (active ? "Active" : "Inactive") << std::endl;
}

void CustomWindow::OnWindowMove(const PIL::Window* w, const PIL::Point& oldPos, const PIL::Point& newPos)
{
	std::cout << w->GetWindowTitle() << " Move From (" << oldPos.x << "," << oldPos.y << ") to (" << newPos.x << "," << newPos.y << ") " << std::endl;
}

void CustomWindow::OnWindowResize(const PIL::Window* w, const PIL::Size& oldSize, const PIL::Size& newSize)
{
	std::cout << w->GetWindowTitle() << " Resize From (" << oldSize.x << "," << oldSize.y << ") to (" << newSize.x << "," << newSize.y << ") " << std::endl;
}


void WindowListener::OnCreate(const PIL::Window* w)
{
	if (w)
	{
		std::cout << "WindowListener: " << w->GetWindowTitle() << " Create" << std::endl;
	}
}

void WindowListener::OnSetActive(const PIL::Window* w, bool active)
{
	if (w)
	{
		std::cout << "WindowListener: " << w->GetWindowTitle() << " Active Change" << std::endl;
	}
}

void WindowListener::OnWindowMove(const PIL::Window* w, const PIL::Point& oldPos, const PIL::Point& newPos)
{
	if (w)
	{
		std::cout << "WindowListener: " << w->GetWindowTitle() << " Move" << std::endl;
	}
}

void WindowListener::OnWindowResize(const PIL::Window* w, const PIL::Size& oldSize, const PIL::Size& newSize)
{
	if (w)
	{
		std::cout << "WindowListener: " << w->GetWindowTitle() << " Resize" << std::endl;
	}
}

bool WindowListener::OnClosing(const PIL::Window* w)
{
	if (w)
	{
		std::cout << "WindowListener: Close " << w->GetWindowTitle() << " ? (y/n)" << std::endl;
		char in;
		std::cin >> in;
		if (in == 'y')
		{
			return true;
		}
		else return false;
	}
	else return true;
}
