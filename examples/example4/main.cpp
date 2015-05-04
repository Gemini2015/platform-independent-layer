#include "PILInclude.h"
#include "CustomWindow.h"

#if defined(PLATFORM_WIN32)

#include <conio.h>

#elif defined(PLATFORM_LINUX)

#include <stdio.h>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>

int _kbhit(void)
{
	struct termios oldt, newt;
	int ch;
	int oldf;
	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);
	ch = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);
	if (ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}
	return 0;
}

#endif

#include <iostream>

int main()
{
	PIL::Root* root = new PIL::Root();
	//WindowListener* listener = new WindowListener();
	PIL::WindowManager* wm = root->GetWindowManger();
	//wm->AddListener(listener);
	CustomWindow* window = new CustomWindow("Window - 1");
	if (window == NULL)
		return -1;
	HRESULT hr = window->Create();
	if (FAILED(hr))
	{
		delete window;
		window = NULL;
	}

	std::cout << "Window Created\n";
	std::cout << "Show Window\n";
	window->SetHidden(false);

	CustomWindow* window2 = new CustomWindow("Window - 2");
	if (window2 == NULL)
		return -1;
	hr = window2->Create();
	if (FAILED(hr))
	{
		delete window2;
		window2 = NULL;
	}

	std::cout << "Show Window \n";
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
	if (window)
		delete window;
	if (window2)
		delete window2;
	/*if (listener)
		delete listener;*/
	return 0;
}

