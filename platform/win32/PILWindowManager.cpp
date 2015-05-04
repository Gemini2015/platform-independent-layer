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

}