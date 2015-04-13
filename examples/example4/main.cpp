#include "PILInclude.h"
#include "CustomWindow.h"

#if defined(PLATFORM_WIN32)

#include <conio.h>

#endif

#include <iostream>

int main()
{
	PIL::Root* root = new PIL::Root();
	WindowListener* listener = new WindowListener();
	PIL::WindowManager* wm = root->GetWindowManger();
	wm->AddListener(listener);
	CustomWindow* window = CustomWindow::Create("Window - 1");
	if (window == NULL)
		return -1;

	char in;
	std::cout << "Window Created\n";
	std::cout << "Show Window ?\n";
	std::cin >> in;
	window->SetHidden(false);

	CustomWindow* window2 = CustomWindow::Create("Window - 2");
	std::cout << "Show Window ?\n";
	std::cin >> in;
	window2->SetHidden(false);

	while (1)
	{
		wm->HandleMessage();
		if (_kbhit())
		{
			break;
		}
	}

	if (root)
		delete root;
	return 0;
}

