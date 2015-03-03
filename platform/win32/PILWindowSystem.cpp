#include "PILWindowSystem.h"

namespace PIL
{


	Window::Window(std::string name, int32 x, int32 y, uint32 width, uint32 height)
	{

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

	}

	HRESULT Window::Destory()
	{

	}

}