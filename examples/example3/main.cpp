#include "PILInclude.h"
#include <stdio.h>

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
	if(ch != EOF)
	{
		ungetc(ch, stdin);
		return 1;
	}
	return 0;
}

#endif

int main(int argc, char** argv)
{
	PIL::WindowManager* wm = new PIL::WindowManager();
	if (wm == NULL)
		return -1;

	PIL::Window *w = NULL;

	HRESULT hr = wm->NewWindow("Hello", 100, 100, 320, 200, NULL, &w);
	if (w)
	{
		w->ShowWindow(true);
	}

	PIL::Window *w2 = NULL;
	hr = wm->NewWindow("Hello - 1", 200, 200, 320, 200, NULL, &w2);
	if (w2)
	{
		w2->ShowWindow(true);
	}
	printf("Press Anykey to exit.\n");
	while (1)
	{
		wm->HandleMessage();
		if (_kbhit())
		{
			break;
		}
	}
	delete wm;
	return 0;
}
