#ifndef _PIL_WINDOW_MANAGER_H_
#define _PIL_WINDOW_MANAGER_H_

#include "PILPrerequisites.h"

#include "PILWindow.h"

namespace PIL
{
	class WindowManager
	{
	public:
		WindowManager() {}
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
			window->SetWindowManager(this);
			if (window == NULL)
				return E_OUTOFMEMORY;

			if (FAILED(window->Create()))
			{
				delete window;
				return E_FAIL;
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

		HRESULT DeleteWindow(Window* window)
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

		HRESULT DeleteWindow(std::string name)
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

		HRESULT ChangeActiveWindow(Window* w, bool bActive)
		{
			if (w == NULL)
				return E_INVALIDARG;
			Window_Map::iterator it = mWindowMap.begin();
			while (it != mWindowMap.end())
			{
				if (it->second)
				{
					it->second->OnActiveChange(w, bActive);
				}
				it++;
			}
			return S_OK;
		}

		HRESULT MoveOrResizeWindow(Window *w)
		{
			Window_Map::iterator it = mWindowMap.begin();
			while (it != mWindowMap.end())
			{
				if (it->second)
				{
					it->second->OnMoveOrResize(w);
				}
				it++;
			}
			return S_OK;
		}

		HRESULT HandleMessage()
		{
			Window_Map::iterator it = mWindowMap.begin();
			while (it != mWindowMap.end())
			{
				if (it->second)
				{
					it->second->HandleMessage();
				}
				it++;
			}
			return S_OK;
		}

	protected:
		typedef std::map<std::string, Window*> Window_Map;
		Window_Map mWindowMap;
	};
}

#endif
