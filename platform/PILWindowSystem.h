#ifndef _PIL_WINDOW_SYSTEM_H_
#define _PIL_WINDOW_SYSTEM_H_

#include "PILPrerequisites.h"

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

	private:
		Window(std::string name, int32 x, int32 y, uint32 width, uint32 height, NameValue_Map *param);
		~Window();

		HRESULT Create();

		HRESULT Destory();

	protected:
		uint32 mID;
		WindowStatus mStatus;
		std::string mName;
		int32 mLeft;
		int32 mTop;
		uint32 mWidth;
		uint32 mHeight;
		NameValue_Map mParamList;

#if defined(PLATFORM_WIN32)
		HINSTANCE mHInstance;
		HWND mHWnd;
#else
#endif

	private:
		static uint32 sWindowCounter;
	};

	class WindowManager
	{
	public:
		WindowManager();
		~WindowManager()
		{
			Window_Map::iterator it = mWindowMap.begin();
			while (it != mWindowMap.end())
			{
				delete it->second;
				it = mWindowMap.erase(it);
			}
		}

		HRESULT NewWindow(std::string name, int32 x, int32 y, uint32 width, uint32 height, NameValue_Map *param, Window** ppWindow)
		{
			if (name.empty() || width < 0 || height < 0 || ppWindow == NULL)
				return E_INVALIDARG;
			Window_Map::iterator it = mWindowMap.find(name);
			if (it != mWindowMap.end())
			{
				(*ppWindow) = it->second;
				return S_FALSE;
			}

			Window* window = new Window(name, x, y, width, height, param);
			if (window == NULL)
				return E_OUTOFMEMORY;

			if (FAILED(window->Create()))
			{
				delete window;
			}
			mWindowMap[name] = window;
			(*ppWindow) = window;
			return S_OK;
		}

		HRESULT GetWindow(std::string name, Window** ppWindow)
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

		HRESULT DestoryWindow(Window* window)
		{
			bool bFound = false;
			Window_Map::iterator it = mWindowMap.begin();
			while (it != mWindowMap.end())
			{
				if (it->second == window)
				{
					bFound = true;
					break;
				}
				it++;
			}
			if (bFound)
			{
				delete it->second;
				mWindowMap.erase(it);
				return S_OK;
			}
			else return S_FALSE;
		}

		HRESULT DestoryWindow(std::string name)
		{
			Window_Map::iterator it = mWindowMap.find(name);
			if (it != mWindowMap.end())
			{
				delete it->second;
				mWindowMap.erase(it);
				return S_OK;
			}
			else return S_FALSE;
		}

	protected:
		typedef std::map<std::string, Window*> Window_Map;
		Window_Map mWindowMap;
	};
}

#endif