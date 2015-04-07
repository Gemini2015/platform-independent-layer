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
		if (mStatus != WS_Destoryed)
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

		mStatus = WS_Hide;
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
		mStatus = WS_Destoryed;
		mIsActive = false;
		mIsFullScreen = false;
		mIsClosed = true;
		return S_OK;
	}

	bool Window::ShowWindow(bool bShow)
	{
		if ((bShow == true && mStatus == WS_Show) ||
			(bShow == false && mStatus == WS_Hide))
			return true;
		if (mHWnd != NULL)
		{
			if (bShow)
			{
				::ShowWindow(mHWnd, SW_SHOW);
				mStatus = WS_Show;
				mIsActive = true;
			}
			else
			{
				mStatus = WS_Hide;
				mIsActive = false;
			}
			return true;
		}
		else return false;
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
		else
		{
			mStatus = WS_Hide;
		}
		mIsActive = bActive;

		if (mIsFullScreen)
		{
			if (bActive == false)
			{
				::ShowWindow(mHWnd, SW_SHOWMINNOACTIVE);
			}
			else
			{
				::ShowWindow(mHWnd, SW_SHOWNORMAL);
			}
		}
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

	bool Window::IsVisible() const
	{
		return (mHWnd && !IsIconic(mHWnd));
	}

	void Window::SetHidden(bool hidden)
	{
		mIsHidden = hidden;
		if (hidden)
			::ShowWindow(mHWnd, SW_HIDE);
		else
			::ShowWindow(mHWnd, SW_SHOWNORMAL);
	}

	bool Window::IsActive()
	{
		if (IsFullScreen())
			return IsVisible();

		return mIsActive && IsVisible();
	}

	bool Window::IsHidden() const
	{
		return mIsHidden;
	}

	void Window::SetFullScreen(bool fullScreen, uint32 width, uint32 height)
	{

	}

	bool Window::IsVSyncEnabled() const
	{
		return mIsVSync;
	}

	void Window::SetVSyncEnabled(bool vSync)
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
	}

	bool Window::IsClosed() const
	{
		return mIsClosed;
	}

	void Window::SwapBuffers(bool waitForVSync)
	{
		::SwapBuffers(mHDC);
	}

	bool Window::IsFullScreen() const
	{
		return mIsFullScreen;
	}

}
