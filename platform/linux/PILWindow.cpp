#include "PILWindow.h"
#include "PILWindowManager.h"

namespace PIL
{
	uint32 Window::sWindowCounter = 0;
	Display* Window::sDisplay = NULL;
	int32 Window::sDisplayRefCount = 0;

	Window::Window(std::string name, int32 x, int32 y, uint32 width, uint32 height, NameValue_Map *param)
		: mID(sWindowCounter++)
		, mName(name)
		, mTitle(name)
		, mLeft(x)
		, mTop(y)
		, mWidth(width)
		, mHeight(height)
		, mIsActive(false)
		, mIsFullScreen(false)
		, mIsHidden(true)
		, mIsVSync(false)
		, mIsClosed(true)
		, mIsVisible(false)
		, mWindow(0)
		, mAtomDeleteWindow(0)
		, mWindowManager(NULL)
	{
		if(param) mParamList = *param;
	}

	Window::~Window()
	{
		if (mIsClosed == false)
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

		mAtomDeleteWindow = XInternAtom(sDisplay, "WM_DELETE_WINDOW", false);
		XSetWMProtocols(sDisplay, mWindow, &mAtomDeleteWindow, 1);

		XStoreName(sDisplay, mWindow, mTitle.c_str());

		XSelectInput(sDisplay, mWindow, StructureNotifyMask | VisibilityChangeMask | FocusChangeMask | ExposureMask);

		XUnmapWindow(sDisplay, mWindow);
		mIsClosed = false;

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
		mIsActive = false;
		mIsClosed = true;
		return S_OK;
	}

	void Window::HandleMessage()
	{
		if(sDisplay == NULL || mWindow == 0)
			return ;

		if(IsClosed())
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
				break;
			}
			case FocusOut:
			{
				if(mWindowManager)
				{
					mWindowManager->ChangeActiveWindow(this, false);
				}
				break;
			}
			case ConfigureNotify:
			{
				if(mWindowManager)
				{
					mWindowManager->MoveOrResizeWindow(this);
				}
				break;
			}
			default:break;
			}
		}

		// XCheckWindowEvent doesn't receive ClientMessage
		while(XCheckTypedWindowEvent(sDisplay, mWindow, ClientMessage, &event))
		{
			if((Atom)(event.xclient.data.l[0]) == mAtomDeleteWindow && mWindowManager != NULL)
			{
				mWindowManager->DeleteWindow(this);
			}
		}
	}

	HRESULT Window::SetWindowTitle(std::string title)
	{
		if(sDisplay == NULL || mWindow == 0L)
			return E_INVALIDARG;

		if(IsClosed())
			return E_FAIL;

		mTitle = title;
		XStoreName(sDisplay, mWindow, mTitle.c_str());
		return S_OK;
	}

	HRESULT Window::SetWindowPosition(int32 left, int32 top)
	{
		if(sDisplay == NULL || mWindow == 0L)
			return E_INVALIDARG;

		if(IsClosed())
			return E_FAIL;

		XMoveWindow(sDisplay, mWindow, left, top);
		return S_OK;
	}

	HRESULT Window::SetWindowSize(uint32 width, uint32 height)
	{
		if(sDisplay == NULL || mWindow == 0L)
			return E_INVALIDARG;

		if(IsClosed())
			return E_FAIL;

		if(mWidth == width && mHeight == height)
			return S_OK;

		if(width != 0 && height != 0)
		{
			XResizeWindow(sDisplay, mWindow, width, height);
			return S_OK;
		}
		else
		{
			return E_INVALIDARG;
		}
	}

	HRESULT Window::SetVisible(bool visible)
	{
		mIsVisible = visible;
		return S_OK;
	}

	bool Window::IsVisible() const
	{
		return mIsVisible;
	}

	HRESULT Window::SetActive(bool bActive)
	{
		mIsActive = bActive;
		return S_OK;
	}

	bool Window::IsActive() const
	{
		return mIsActive && IsVisible();
	}

	HRESULT Window::SetHidden(bool hidden)
	{
		if(sDisplay == NULL || mWindow == 0L)
			return E_INVALIDARG;

		if(IsClosed())
			return E_FAIL;

		if(mIsHidden == hidden)
			return S_OK;

		mIsHidden = hidden;
		if (hidden)
		{
			XUnmapWindow(sDisplay, mWindow);
		}
		else
		{
			XMapWindow(sDisplay, mWindow);
		}
		return S_OK;
	}

	bool Window::IsHidden() const
	{
		return mIsHidden;
	}

	HRESULT Window::SetFullScreen(bool fullScreen, uint32 width, uint32 height)
	{
		if(IsClosed())
			return S_FALSE;

		if(mIsFullScreen == fullScreen && mWidth == width && mHeight == height)
			return S_OK;

		return S_OK;
	}

	bool Window::IsFullScreen() const
	{
		return mIsFullScreen;
	}

	HRESULT SetVSyncEnabled(bool vSync)
	{
		return S_OK;
	}

	bool Window::IsVSyncEnabled() const
	{
		return true;
	}

	bool Window::IsClosed() const
	{
		return mIsClosed;
	}

	HRESULT Window::SwapBuffers(bool waitForVSync)
	{
		return S_OK;
	}

	void Window::OnActiveChange(Window* w, bool bActive)
	{
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

	void Window::AddListener(IWindowEventListener* listener)
	{
		IWindowEventListenerList::iterator it = std::find(mListenerList.begin(), mListenerList.end(), listener);
		if(it == mListenerList.end())
		{
			mListenerList.push_back(listener);
		}
	}

	void Window::RemoveListener(IWindowEventListener* listener)
	{
		IWindowEventListenerList::iterator it = std::find(mListenerList.begin(), mListenerList.end(), listener);
		if(it != mListenerList.end())
		{
			mListenerList.erase(it);
		}
	}
}
