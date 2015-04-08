#include "PILWindow.h"
#include "PILWindowManager.h"

namespace PIL
{
uint32 Window::sWindowCounter = 0;
	Display* Window::sDisplay = NULL;
	int32 Window::sDisplayRefCount = 0;

	Window::Window(std::string name, int32 x, int32 y, uint32 width, uint32 height, NameValue_Map *param)
	{
		mID = sWindowCounter++;
		mName = name;
		mTitle = name;
		mLeft = x;
		mTop = y;
		mWidth = width;
		mHeight = height;
		if(param) mParamList = *param;
		mWindowManager = NULL;
		mIsActive = false;
		mIsFullScreen = false;
		mStatus = WS_None;
		mWindow = 0L;
	}

	Window::~Window()
	{
		if (mStatus != WS_Destoryed)
		{
			Destory();
		}
	}

	HRESULT Window::Create()
	{
		if (mName.empty() || mWidth < 0 || mHeight < 0)
			return E_INVALIDARG;

		if (sDisplay == NULL)
		{
			sDisplay = XOpenDisplay(NULL);
			if (sDisplay == NULL)
				return E_FAIL;
		}
		Visual *visual = NULL;
		visual = DefaultVisual(sDisplay, 0);

		mWindow = XCreateWindow(sDisplay, DefaultRootWindow(sDisplay),
				mLeft, mTop, mWidth, mHeight,
				0, CopyFromParent, CopyFromParent, visual, 0, NULL);

		if(mWindow == 0L)
			return E_FAIL;

		XStoreName(sDisplay, mWindow, mTitle.c_str());

		XSelectInput(sDisplay, mWindow, StructureNotifyMask | VisibilityChangeMask | FocusChangeMask | ExposureMask);

		XUnmapWindow(sDisplay, mWindow);
		mStatus = WS_Hide;

		sDisplayRefCount++;
		return S_OK;
	}

	HRESULT Window::Destory()
	{
		if(sDisplay != NULL && mWindow != 0L)
		{
			XDestroyWindow(sDisplay, mWindow);
			mWindow = 0L;
			sDisplayRefCount--;
			if(sDisplayRefCount == 0)
			{
				XCloseDisplay(sDisplay);
				sDisplay = NULL;
			}
		}
		mStatus = WS_Destoryed;
		mIsActive = false;
		mIsFullScreen = false;
		return S_OK;
	}

	bool Window::ShowWindow(bool bShow)
	{
		if(sDisplay == NULL || mWindow == 0L)
			return false;

		if (bShow)
		{
			XMapWindow(sDisplay, mWindow);
			mStatus = WS_Show;
		}
		else
		{
			XUnmapWindow(sDisplay, mWindow);
			mStatus = WS_Hide;
		}
		return true;
	}

	void Window::HandleMessage()
	{

		if(sDisplay == NULL || mWindow == 0)
			return ;

		XEvent event;

		while (XCheckWindowEvent(sDisplay, mWindow,
				StructureNotifyMask | VisibilityChangeMask | FocusChangeMask | ExposureMask,
				&event))
		{
			switch(event.type)
			{
			case Expose:
			{
				const char *msg = "Hello X11";
				XFillRectangle(sDisplay, mWindow, DefaultGC(sDisplay, DefaultScreen(sDisplay)), 20, 20, 10, 10);
				XDrawString(sDisplay, mWindow, DefaultGC(sDisplay, DefaultScreen(sDisplay)), 50, 50, msg, strlen(msg));
				break;
			}
			case DestroyNotify:
			{
				if(mWindowManager)
				{
					mWindowManager->DeleteWindow(this);
				}
				break;
			}
			case MapNotify:
			{
				if(mWindowManager)
				{
					mWindowManager->ChangeActiveWindow(this, true);
				}
				break;
			}
			case UnmapNotify:
			{	//win->setVisible( false );
				if(mWindowManager)
				{
					mWindowManager->ChangeActiveWindow(this, false);
				}
				break;
			}
			case FocusIn:
			{
				if(mWindowManager)
				{
					mWindowManager->ChangeActiveWindow(this, true);
				}
			}
			case FocusOut:
			{
				if(mWindowManager)
				{
					mWindowManager->ChangeActiveWindow(this, false);
				}
			}
			case ConfigureNotify:
			{
				if(mWindowManager)
				{
					mWindowManager->MoveOrResizeWindow(this);
				}
			}
			default:
				break;
			}
		}
	}

	HRESULT Window::SetWindowTitle(std::string title)
	{
		if(sDisplay == NULL || mWindow == 0L)
			return E_INVALIDARG;
		mTitle = title;
		XStoreName(sDisplay, mWindow, mTitle.c_str());
		return S_OK;
	}

	void Window::OnActiveChange(Window* w, bool bActive)
	{
		if (w == this)
		{
			SetActive(bActive);
		}
	}

	void Window::SetActive(bool bActive)
	{
		if (bActive)
		{
			mStatus = WS_Show;
		}
		mIsActive = bActive;
	}

	void Window::OnMoveOrResize(Window *w)
	{
		if(this == w)
		{
			XWindowAttributes attr;
			::Window root, parent, *pchild = NULL, child;
			uint32 nchild = 0;
			int ox = 0, oy = 0;
			XQueryTree(sDisplay, mWindow, &root, &parent, &pchild, &nchild);
			XTranslateCoordinates(sDisplay, mWindow, root, 0, 0, &ox, &oy, &child);
			if(XGetWindowAttributes(sDisplay, mWindow, &attr))
			{
				mLeft = ox - attr.x;
				mTop = oy - attr.y;
				mWidth = attr.width;
				mHeight = attr.height;
			}
		}
	}
}
