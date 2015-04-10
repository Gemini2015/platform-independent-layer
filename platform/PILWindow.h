#ifndef _PIL_WINDOW_H_
#define _PIL_WINDOW_H_

#include "PILPrerequisites.h"

#if defined(PLATFORM_LINUX)

#include <X11/X.h>
#include <X11/Xlib.h>

#endif

namespace PIL
{
	class IWindowEventListener
	{
		virtual ~IWindowEventListener();

		// after window created
		virtual void OnCreate(IWindowEventListener* sender, Window* w) = 0;

		// before window destroy
		virtual void OnDestroy(IWindowEventListener* sender, Window* w) = 0;

		// focus change
		virtual void OnSetActive(IWindowEventListener* sender, Window* w, bool active) = 0;

		// visibility change
		virtual void OnSetVisible(IWindowEventListener* sender, Window* w, bool visible) = 0;

		// window move
		virtual void OnWindowMove(IWindowEventListener* sender, Window* w, Point oldPos, Point newPos) = 0;

		// window resize
		virtual void OnWindowResize(IWindowEventListener* sender, Window* w, Size oldSize, Size newSize) = 0;

	};

	typedef std::vector<IWindowEventListener*> IWindowEventListenerList;

	class Window
	{
		friend class WindowManager;

	public:

		HRESULT SetWindowTitle(std::string title);

		std::string GetWindowTitle() const { return mTitle; }

		HRESULT SetWindowPosition(int32 left, int32 top);

		Point GetWindowPosition() const	{ return Point(mLeft, mTop); }

		HRESULT SetWindowSize(uint32 width, uint32 height);

		Size GetWindowSize() const { return Size(mWidth, mHeight); }

		HRESULT SetVisible(bool visible);

		bool IsVisible() const;

		HRESULT SetActive(bool active);

		bool IsActive() const;

		HRESULT SetHidden(bool hidden);

		bool IsHidden() const;

		HRESULT SetFullScreen(bool fullScreen, uint32 width, uint32 height);

		bool IsFullScreen() const;

		HRESULT SetVSyncEnabled(bool vSync);

		bool IsVSyncEnabled() const;

		bool IsClosed() const;

		HRESULT SwapBuffers(bool waitForVSync);


		WindowManager* GetWindowManager() const	{ return mWindowManager; }

		void AddListener(IWindowEventListener* listener);

		void RemoveListener(IWindowEventListener* listener);

	private:
		Window(std::string name, int32 x, int32 y, uint32 width, uint32 height, NameValue_Map *param);

		~Window();

		void SetWindowManager(WindowManager* wm)
		{
			mWindowManager = wm;
		}

		HRESULT Create();

		HRESULT Destory();

		void HandleMessage();

		void OnActiveChange(Window* w, bool bActive);

		void OnMoveOrResize(Window* w);

	protected:
		uint32 mID;
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
		bool mIsVisible;

		IWindowEventListenerList mListenerList;

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
		Atom mAtomDeleteWindow;
#endif

	private:
		static uint32 sWindowCounter;
		WindowManager* mWindowManager;
	};
}

#endif
