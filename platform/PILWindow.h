#ifndef _PIL_WINDOW_H_
#define _PIL_WINDOW_H_

#include "PILPrerequisites.h"

#if defined(PLATFORM_LINUX)

#include <X11/Xlib.h>

#endif

namespace PIL
{
	class Window
	{
		friend class WindowManager;
	public:
		enum WindowStatus
		{
			WS_None = 0,
			WS_Hide,
			WS_Show,
			WS_Destoryed,
		};

	public:

		bool ShowWindow(bool bShow);

		void HandleMessage();

		HRESULT SetWindowTitle(std::string title);
		std::string GetWindowTitle()
		{
			return mTitle;
		}

		Point GetWindowPosition()
		{
			return Point(mLeft, mTop);
		}

		Size GetCanvasSize()
		{
			return Size(mWidth, mHeight);
		}

		bool IsVisible() const;

		void SetActive(bool bActive);

		bool IsActive();

		bool IsHidden() const;

		void SetHidden(bool hidden);

		bool IsFullScreen() const;

		void SetFullScreen(bool fullScreen, uint32 width, uint32 height);

		bool IsVSyncEnabled() const;

		void SetVSyncEnabled(bool vSync);

		bool IsClosed() const;

		void SwapBuffers(bool waitForVSync);

		WindowManager* GetWindowManager()
		{
			return mWindowManager;
		}

	private:
		Window(std::string name, int32 x, int32 y, uint32 width, uint32 height, NameValue_Map *param);
		~Window();

		void SetWindowManager(WindowManager* wm)
		{
			mWindowManager = wm;
		}

		HRESULT Create();

		HRESULT Destory();

		void OnActiveChange(Window* w, bool bActive);
		void OnMoveOrResize(Window* w);

	protected:
		uint32 mID;
		WindowStatus mStatus;
		std::string mName;
		std::string mTitle;
		int32 mLeft;
		int32 mTop;
		uint32 mWidth;
		uint32 mHeight;
		NameValue_Map mParamList;
		bool mIsActive;
		bool mIsFullScreen;
		bool mIsHidden;
		bool mIsVSync;
		bool mIsClosed;

#if defined(PLATFORM_WIN32)
		HINSTANCE mHInstance;
		HWND mHWnd;
		HDC mHDC;
		HGLRC mGLRC;

	public:
		static LRESULT CALLBACK WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
#else
		static Display *sDisplay;
		static int32 sDisplayRefCount;
		::Window mWindow;
#endif

	private:
		static uint32 sWindowCounter;
		WindowManager* mWindowManager;
	};
}

#endif
