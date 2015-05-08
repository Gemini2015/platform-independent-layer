#include "PILInclude.h"
#include "GLDemo.h"

int main()
{
	PIL::Root* root = new PIL::Root();

	GLDemo* demo = new GLDemo();

	try
	{
		demo->Setup();

		demo->Run();

		demo->Shutdown();
	}
	catch (std::exception e)
	{
		std::cout << e.what() << std::endl;
	}

	if (demo)
		delete demo;
	if (root)
		delete root;
}