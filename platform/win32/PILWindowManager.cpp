#include "PILWindowManager.h"

namespace PIL
{

	PIL::WindowManager::Window_Map WindowManager::mWindowMap;

	LRESULT CALLBACK WindowManager::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
	{
		if (uMsg == WM_CREATE)
		{
			SetWindowLongPtr(hWnd, GWLP_USERDATA, (LONG_PTR)(((LPCREATESTRUCT)lParam)->lpCreateParams));
			return 0;
		}

		Window* w = (Window*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
		if (!w)
			return DefWindowProc(hWnd, uMsg, wParam, lParam);

		WindowManager* wm = w->GetWindowManager();

		switch (uMsg)
		{
		case WM_ACTIVATE:
		{
			bool active = (LOWORD(wParam)) != WA_INACTIVE;
			w->OnActiveChange(w, active);
			if (wm)
			{
				wm->NotifyWindowActiveChange(w, active);
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
			if (wm)
			{
				wm->NotifyWindowMove(w);
			}
		}
			break;
		case WM_SIZE:
		{
			w->OnMoveOrResize(w);
			if (wm)
			{
				wm->NotifyWindowResize(w);
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
			if (wm)
			{
				bool ret = wm->NotifyWindowClosing(w);
				if (!ret) return 0;

				w->NotifyWindowDestroy(w);
				wm->DeleteWindow(w);
				return 0;
			}
		}
			break;
		default:
			break;
		}
		return DefWindowProc(hWnd, uMsg, wParam, lParam);

	}

	HRESULT WindowManager::NewWindow(std::string name, int32 x, int32 y, uint32 width, uint32 height, NameValue_Map *param /*= NULL*/, WindowObject* userWindow /*= NULL*/, Window** ppWindow /*= NULL*/)
	{
		if (name.empty() || width < 0 || height < 0)
			return E_INVALIDARG;
		Window_Map::iterator it = mWindowMap.find(name);
		if (it != mWindowMap.end())
		{
			(*ppWindow) = it->second;
			return S_FALSE;
		}

		Window* window = new Window(name, x, y, width, height, param);
		window->SetWindowManager(this);
		if (window == NULL)
			return E_OUTOFMEMORY;

		if (FAILED(window->Create()))
		{
			delete window;
			return E_FAIL;
		}
		window->BindUserWindow(userWindow);
		mWindowMap[name] = window;
		if (ppWindow != NULL) (*ppWindow) = window;
		return S_OK;
	}

	WindowManager::WindowManager()
	{

	}

	WindowManager::~WindowManager()
	{
		Window_Map::iterator it = mWindowMap.begin();
		while (it != mWindowMap.end())
		{
			delete it->second;
			it = mWindowMap.erase(it);
		}
	}

	HRESULT WindowManager::GetWindow(std::string name, Window** ppWindow)
	{
		if (name.empty() || ppWindow == NULL)
			return E_INVALIDARG;

		Window_Map::iterator it = mWindowMap.find(name);
		if (it != mWindowMap.end())
		{
			(*ppWindow) = it->second;
			return S_OK;
		}

		return E_FAIL;
	}

	HRESULT WindowManager::ShutDownWindow(Window* w)
	{
		return S_OK;
	}

	HRESULT WindowManager::DeleteWindow(Window* window)
	{
		if (window == nullptr)
			return E_INVALIDARG;
		bool bFound = false;
		Window_Map::iterator it = mWindowMap.find(window->mName);
		if (it != mWindowMap.end())
		{
			delete it->second;
			mWindowMap.erase(it);
		}
		return S_OK;
	}

	HRESULT WindowManager::DeleteWindow(std::string name)
	{
		Window_Map::iterator it = mWindowMap.find(name);
		if (it != mWindowMap.end())
		{
			if (it->second)
			{
				delete it->second;
				mWindowMap.erase(it);
			}
			return S_OK;
		}
		else return S_FALSE;
	}

	void WindowManager::NotifyWindowActiveChange(const Window* w, bool active)
	{
		IWindowEventListenerList::iterator it = mListenerList.begin();
		while (it != mListenerList.end())
		{
			IWindowEventListener* listener = (*it);
			if (listener != NULL)
			{
				listener->OnSetActive(w, active);
			}
			it++;
		}
	}

	void WindowManager::NotifyWindowMove(const Window* w)
	{
		IWindowEventListenerList::iterator it = mListenerList.begin();
		while (it != mListenerList.end())
		{
			IWindowEventListener* listener = (*it);
			if (listener != NULL)
			{
				listener->OnWindowMove(w, Point(0, 0), Point(0, 0));
			}
			it++;
		}
	}

	void WindowManager::NotifyWindowResize(const Window* w)
	{
		IWindowEventListenerList::iterator it = mListenerList.begin();
		while (it != mListenerList.end())
		{
			IWindowEventListener* listener = (*it);
			if (listener != NULL)
			{
				listener->OnWindowResize(w, Point(0, 0), Point(0, 0));
			}
			it++;
		}
	}

	bool WindowManager::NotifyWindowClosing(const Window* w)
	{
		bool ret = true;
		IWindowEventListenerList::iterator it = mListenerList.begin();
		while (it != mListenerList.end())
		{
			IWindowEventListener* listener = (*it);
			if (listener != NULL)
			{
				if (!(listener->OnClosing(w)))
					ret = false;
			}
			it++;
		}
		return ret;
	}

	void WindowManager::AddListener(IWindowEventListener* listener)
	{
		if (listener == NULL)
			return;
		IWindowEventListenerList::iterator it = std::find(mListenerList.begin(), mListenerList.end(), listener);
		if (it == mListenerList.end())
		{
			mListenerList.push_back(listener);
		}
	}

	void WindowManager::RemoveListener(IWindowEventListener* listener)
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