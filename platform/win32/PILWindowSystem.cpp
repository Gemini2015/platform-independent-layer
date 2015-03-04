#include "PILWindowSystem.h"

namespace PIL
{

	Window::Window(std::string name, int32 x, int32 y, uint32 width, uint32 height, NameValue_Map *param)
	{
		mName = name;
		mLeft = x;
		mTop = y;
		mWidth = width;
		mHeight = height;
		mParamList = *param;
	}

	Window::~Window()
	{
		if (mStatus != WS_Destoryed)
		{
			Destory();
			mStatus = WS_Destoryed;
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
		wndclass.lpfnWndProc = NULL;
		wndclass.cbClsExtra = 0;
		wndclass.cbWndExtra = 0;
		wndclass.hInstance = mHInstance;
		wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
		wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
		wndclass.hbrBackground = (HBRUSH)(COLOR_WINDOW);
		wndclass.lpszMenuName = NULL;
		wndclass.lpszClassName = "PILWindow";
		wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

		if (RegisterClassEx(&wndclass) == 0)
			return E_FAIL;

		mHWnd = CreateWindowEx(NULL, wndclass.lpszClassName, mName.c_str(), WS_OVERLAPPEDWINDOW | WS_VISIBLE,
			mLeft, mTop, mWidth, mHeight, NULL, NULL, mHInstance, NULL);

		if (mHWnd == NULL)
			return E_FAIL;

		mStatus = WS_Hide;
		return S_OK;
	}

	HRESULT Window::Destory()
	{
		UnregisterClass("PILWindow", mHInstance);
		return S_OK;
	}

}