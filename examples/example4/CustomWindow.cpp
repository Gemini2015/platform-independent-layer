#include "CustomWindow.h"
#include "PILInclude.h"

#include <iostream>

CustomWindow::CustomWindow(std::string name)
	: mName(name)
	, mWindow(NULL)
{
}

CustomWindow::~CustomWindow()
{
	mWindow = NULL;
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
	PIL::WindowManager* wm = PIL::Root::SingletonPtr()->GetWindowManger();
	PIL::Window* w = NULL;
	HRESULT hr = wm->NewWindow(mName, 100, 100, 320, 240, NULL, this, &w);
	if (FAILED(hr))
	{
		return E_FAIL;
	}
	mWindow = w;
	w->SetHidden(true);
	w->AddListener(this);
	return S_OK;
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
	if (w && w->GetUserWindow())
	{
		const CustomWindow* window = (const CustomWindow*)w->GetUserWindow();
		std::cout << "WindowListener: " << window->GetName() << " Create" << std::endl;
	}
}

void WindowListener::OnSetActive(const PIL::Window* w, bool active)
{
	if (w && w->GetUserWindow())
	{
		const CustomWindow* window = (const CustomWindow*)w->GetUserWindow();
		std::cout << "WindowListener: " << window->GetName() << " Active Change" << std::endl;
	}
}

void WindowListener::OnWindowMove(const PIL::Window* w, const PIL::Point& oldPos, const PIL::Point& newPos)
{
	if (w && w->GetUserWindow())
	{
		const CustomWindow* window = (const CustomWindow*)w->GetUserWindow();
		std::cout << "WindowListener: " << window->GetName() << " Move" << std::endl;
	}
}

void WindowListener::OnWindowResize(const PIL::Window* w, const PIL::Size& oldSize, const PIL::Size& newSize)
{
	if (w && w->GetUserWindow())
	{
		const CustomWindow* window = (const CustomWindow*)w->GetUserWindow();
		std::cout << "WindowListener: " << window->GetName() << " Resize" << std::endl;
	}
}

bool WindowListener::OnClosing(const PIL::Window* w)
{
	if (w && w->GetUserWindow())
	{
		const CustomWindow* window = (const CustomWindow*)w->GetUserWindow();
		std::cout << "WindowListener: Close " << window->GetName() << " ? (y/n)" << std::endl;
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
