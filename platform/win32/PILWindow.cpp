#include "PILWindow.h"
#include "PILWindowManager.h"

namespace PIL
{
	uint32 Window::sWindowCounter = 0;

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
		, mIsContextInit(false)
		, mHInstance(NULL)
		, mHWnd(NULL)
		, mHDC(NULL)
		, mHGLRC(NULL)
		, mWindowManager(NULL)
	{
		if (param) mParamList = *param;
	}

	Window::~Window()
	{
		if (mIsClosed == false)
		{
			Destroy();
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

		mHWnd = CreateWindowEx(NULL, wndclass.lpszClassName, mTitle.c_str(), WS_OVERLAPPEDWINDOW,
			mLeft, mTop, mWidth, mHeight, NULL, NULL, mHInstance, this);

		if (mHWnd == NULL)
		{
			uint32 error = GetLastError();
			return E_FAIL;
		}

		if (SUCCEEDED(InitContext()))
		{
			mIsContextInit = true;
		}

		mIsClosed = false;

		NotifyWindowCreate(this);
		return S_OK;
	}

	void Window::NotifyWindowCreate(const Window* w)
	{
		IWindowEventListenerList::iterator it = mListenerList.begin();
		while (it != mListenerList.end())
		{
			IWindowEventListener* listener = (*it);
			if (listener != NULL)
			{
				listener->OnCreate(w);
			}
			it++;
		}
	}

	HRESULT Window::InitContext()
	{
		mHDC = GetDC(mHWnd);
		if (FAILED(InitPixelFormat()))
		{
			//Destory();
			return E_FAIL;
		}

		if (!(mHGLRC = wglCreateContext(mHDC)))
		{
			return E_FAIL;
		}

		if (!wglMakeCurrent(mHDC, mHGLRC))
		{
			return E_FAIL;
		}

		return S_OK;
	}

	HRESULT Window::Destroy()
	{
		NotifyWindowDestroy(this);
		if (mHGLRC)
		{
			wglDeleteContext(mHGLRC);
			mHGLRC = NULL;
		}
		if (mHDC)
		{
			ReleaseDC(mHWnd, mHDC);
			mHDC = NULL;
		}
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

	void Window::NotifyWindowDestroy(const Window* w)
	{
		IWindowEventListenerList::iterator it = mListenerList.begin();
		while (it != mListenerList.end())
		{
			IWindowEventListener* listener = (*it);
			if (listener != NULL)
			{
				listener->OnDestroy(w);
			}
			it++;
		}
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
			bool active = (LOWORD(wParam)) != WA_INACTIVE;
			w->OnActiveChange(w, active);
			if (w->GetWindowManager())
			{
				w->GetWindowManager()->NotifyWindowActiveChange(w);
			}
		}
			break;
		case WM_KEYDOWN:
		{
		}
			break;
		case WM_MOVE:
		{
			w->OnMoveOrResize(w);
			if (w->GetWindowManager())
			{
				w->GetWindowManager()->NotifyWindowMove(w);
			}
		}
			break;
		case WM_SIZE:
		{
			w->OnMoveOrResize(w);
			if (w->GetWindowManager())
			{
				w->GetWindowManager()->NotifyWindowResize(w);
			}
		}
			break;
		case WM_DISPLAYCHANGE:
		{
			w->OnMoveOrResize(w);
		}
			break;
		case WM_CLOSE:
		{
			if (w->GetWindowManager())
			{
				bool ret = w->GetWindowManager()->NotifyWindowClosing(w);
				if (!ret) return 0;

				w->GetWindowManager()->DeleteWindow(w);
			}
		}
			break;
		default:
			break;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);
	}

	HRESULT Window::InitPixelFormat()
	{
		UINT iformat = -1;
		if (mHDC == NULL)
			return E_INVALIDARG;

		PIXELFORMATDESCRIPTOR pfd;
		memset(&pfd, 0, sizeof(PIXELFORMATDESCRIPTOR));
		pfd.nSize = sizeof(PIXELFORMATDESCRIPTOR);
		pfd.nVersion = 1;
		pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
		pfd.iPixelType = PFD_TYPE_RGBA;
		pfd.cColorBits = 24;
		pfd.cAlphaBits = 8;
		pfd.cDepthBits = 16;

		if (!(iformat = ChoosePixelFormat(mHDC, &pfd)))
			return E_FAIL; 

		if (!SetPixelFormat(mHDC, iformat, &pfd))
			return E_FAIL;
		return S_OK;
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

	void Window::OnActiveChange(const Window* w, bool bActive)
	{
		if (w == this && mHWnd != NULL)
		{
			mIsActive = bActive;
			NotifyActiveChange(bActive);
		}
	}

	void Window::NotifyActiveChange(bool active)
	{
		IWindowEventListenerList::iterator it = mListenerList.begin();
		while (it != mListenerList.end())
		{
			IWindowEventListener* listener = (*it);
			if (listener != NULL)
			{
				listener->OnSetActive(this, active);
			}
			it++;
		}
	}

	void Window::OnMoveOrResize(const Window *w)
	{
		if (w == this)
		{
			if (mHWnd)
			{
				RECT rect;
				GetWindowRect(mHWnd, &rect);

				if (mLeft != rect.left || mTop != rect.top)
				{
					Point oldPos(mLeft, mTop);
					mLeft = rect.left;
					mTop = rect.top;
					NotifyWindowMove(oldPos, Point(rect.left, rect.top));
				}

				GetClientRect(mHWnd, &rect);
				uint32 newWidth = rect.right - rect.left;
				uint32 newHeight = rect.bottom - rect.top;
				if (mWidth != newWidth || mHeight != newHeight)
				{
					Point oldSize(mWidth, mHeight);
					mWidth = rect.right - rect.left;
					mHeight = rect.bottom - rect.top;
					NotifyWindowResize(oldSize, Point(newWidth, newHeight));
				}
			}
		}
	}

	void Window::NotifyWindowMove(const Point& oldPos, const Point& newPos)
	{
		IWindowEventListenerList::iterator it = mListenerList.begin();
		while (it != mListenerList.end())
		{
			IWindowEventListener* listener = (*it);
			if (listener != NULL)
			{
				listener->OnWindowMove(this, oldPos, newPos);
			}
			it++;
		}
	}

	void Window::NotifyWindowResize(const Size& oldSize, const Size& newSize)
	{
		IWindowEventListenerList::iterator it = mListenerList.begin();
		while (it != mListenerList.end())
		{
			IWindowEventListener* listener = (*it);
			if (listener != NULL)
			{
				listener->OnWindowResize(this, oldSize, newSize);
			}
			it++;
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
		if (!wglMakeCurrent(mHDC, mHGLRC))
		{
			// Log Error
		}
		if (old_context && old_context != mHGLRC)
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

	void Window::BindCustomWindow(void* window)
	{
		mCustomWindow = window;
	}

	void* Window::GetCustomWindow() const
	{
		return mCustomWindow;
	}
}
