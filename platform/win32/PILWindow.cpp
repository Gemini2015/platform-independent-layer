#include "PILWindow.h"
#include "PILWindowManager.h"

namespace PIL
{
	uint32 Window::sWindowCounter = 0;

	Window::Window(std::string name, int32 x, int32 y, uint32 width, uint32 height, NameValue_Map *param)
	{
		mID = sWindowCounter++;
		mName = name;
		mTitle = name;
		mLeft = x;
		mTop = y;
		mWidth = width;
		mHeight = height;
		if (param) mParamList = *param;
		mWindowManager = NULL;
		mIsActive = false;
		mIsFullScreen = false;
		mIsClosed = true;
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
		mHInstance = GetModuleHandle(NULL);
		if (mHInstance == NULL)
			return E_FAIL;

		WNDCLASSEX wndclass;
		wndclass.cbSize = sizeof(wndclass);
		wndclass.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC | CS_DBLCLKS;
		wndclass.lpfnWndProc = WindowProc;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = mHInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = "PILWindow";
		wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		RegisterClassEx(&wndclass);

		mHWnd = CreateWindowEx(NULL, wndclass.lpszClassName, mTitle.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			mLeft, mTop, mWidth, mHeight, NULL, NULL, mHInstance, this);

		if (mHWnd == NULL)
		{
			uint32 error = GetLastError();
			return E_FAIL;
		}

		mIsClosed = false;
		return S_OK;
	}

	HRESULT Window::Destory()
	{
		if (mHWnd)
		{
			DestroyWindow(mHWnd);
			mHWnd = NULL;
		}
		UnregisterClass("PILWindow", mHInstance);
		mIsActive = false;
		mIsFullScreen = false;
		mIsClosed = true;
		return S_OK;
	}

	LRESULT CALLBACK Window::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_CREATE)
		{
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)(((LPCREATESTRUCT)lParam)->lpCreateParams));
			return 0;
		}

		Window* w = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (!w)
			return DefWindowProc(hWnd, uMsg, wParam, lParam);

		switch (uMsg)
		{
		case WM_ACTIVATE:
		{
			if (w->GetWindowManager())
			{
				bool bActive = (LOWORD(wParam)) != WA_INACTIVE;
				w->GetWindowManager()->ChangeActiveWindow(w, bActive);
			}
		}
			break;
		case WM_KEYDOWN:
		{
		}
			break;
		case WM_MOVE:
		case WM_SIZE:
		case WM_DISPLAYCHANGE:
		{
			if (w->GetWindowManager())
			{
				w->GetWindowManager()->MoveOrResizeWindow(w);
			}
		}
			break;
		default:
			break;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	void Window::HandleMessage()
	{
		MSG  msg;
		while (PeekMessage(&msg, NULL, 0U, 0U, PM_REMOVE))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	void Window::OnActiveChange(Window* w, bool bActive)
	{
	}

	void Window::OnMoveOrResize(Window *w)
	{
		if (w == this)
		{
			if (mHWnd)
			{
				RECT rect;
				GetWindowRect(mHWnd, &rect);

				mLeft = rect.left;
				mTop = rect.top;

				GetClientRect(mHWnd, &rect);
				mWidth = rect.right - rect.left;
				mHeight = rect.bottom - rect.top;
			}
		}
	}

	HRESULT Window::SetWindowTitle(std::string title)
	{
		if (mHWnd)
		{
			mTitle = title;
			SetWindowText(mHWnd, mTitle.c_str());
			return S_OK;
		}
		else return E_FAIL;
	}

	HRESULT Window::SetWindowPosition(int32 left, int32 top)
	{
		if (mHWnd && !mIsFullScreen)
		{
			SetWindowPos(mHWnd, 0, left, top, 0, 0,
				SWP_NOSIZE | SWP_NOZORDER | SWP_NOACTIVATE);
			return S_OK;
		}
		return E_FAIL;
	}

	HRESULT Window::SetWindowSize(uint32 width, uint32 height)
	{
		if (mHWnd && !mIsFullScreen)
		{
			RECT rc = { 0, 0, width, height };
			AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW | WS_VISIBLE, false);
			width = rc.right - rc.left;
			height = rc.bottom - rc.top;
			SetWindowPos(mHWnd, 0, 0, 0, width, height,
				SWP_NOMOVE | SWP_NOZORDER | SWP_NOACTIVATE);
			return S_OK;
		}
		return E_FAIL;
	}

	HRESULT Window::SetVisible(bool visible)
	{
		mIsVisible = visible;
		return S_OK;
	}

	bool Window::IsVisible() const
	{
		return (mHWnd && !IsIconic(mHWnd));
	}

	HRESULT Window::SetActive(bool active)
	{
		mIsActive = active;

		if (mIsFullScreen)
		{
			if (active == false)
			{
				::ShowWindow(mHWnd, SW_SHOWMINNOACTIVE);
			}
			else
			{
				::ShowWindow(mHWnd, SW_SHOWNORMAL);
			}
		}
		return S_OK;
	}

	bool Window::IsActive() const
	{
		if (IsFullScreen())
			return IsVisible();

		return mIsActive && IsVisible();
	}

	HRESULT Window::SetHidden(bool hidden)
	{
		mIsHidden = hidden;
		if (hidden)
			::ShowWindow(mHWnd, SW_HIDE);
		else
			::ShowWindow(mHWnd, SW_SHOWNORMAL);
		return S_OK;
	}

	bool Window::IsHidden() const
	{
		return mIsHidden;
	}

	HRESULT Window::SetFullScreen(bool fullScreen, uint32 width, uint32 height)
	{
		return S_OK;
	}

	bool Window::IsFullScreen() const
	{
		return mIsFullScreen;
	}

	HRESULT Window::SetVSyncEnabled(bool vSync)
	{
		mIsVSync = vSync;
		HDC old_hdc = wglGetCurrentDC();
		HGLRC old_context = wglGetCurrentContext();
		if (!wglMakeCurrent(mHDC, mGLRC))
		{
			// Log Error
		}
		if (old_context && old_context != mGLRC)
		{
			if (!wglMakeCurrent(old_hdc, old_context))
			{
				// Log Error
			}
		}
		return S_OK;
	}

	bool Window::IsVSyncEnabled() const
	{
		return mIsVSync;
	}

	bool Window::IsClosed() const
	{
		return mIsClosed;
	}

	HRESULT Window::SwapBuffers(bool waitForVSync)
	{
		::SwapBuffers(mHDC);
		return S_OK;
	}

	void Window::AddListener(IWindowEventListener* listener)
	{
		if (listener == NULL)
			return;
		IWindowEventListenerList::iterator it = std::find(mListenerList.begin(), mListenerList.end(), listener);
		if (it == mListenerList.end())
		{
			mListenerList.push_back(listener);
		}
	}

	void Window::RemoveListener(IWindowEventListener* listener)
	{
		if (listener == NULL)
			return;
		IWindowEventListenerList::iterator it = std::find(mListenerList.begin(), mListenerList.end(), listener);
		if (it != mListenerList.end())
		{
			mListenerList.erase(it);
		}
	}

}
