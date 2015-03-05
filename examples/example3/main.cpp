#include "PILInclude.h"
#include <stdio.h>
#include <conio.h>

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