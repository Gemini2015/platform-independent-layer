#include "PILInclude.h"
#include <string.h>
#include <string>
#include <iostream>

int main(int argc, char** argv)
{
	std::string path = "";
	bool recursive = false;
	if (argc >= 2)
	{
		path.assign(argv[1]);
	}
	else path.assign(".");
	if (argc >= 3)
	{
		if (strcmp(argv[2], "-r") == 0)
			recursive = true;
	}

	PIL::FileSystem* fs = new PIL::FileSystem();
	if (fs == NULL)
		return -1;

	PIL::FileInfo_Vec filelist;

	fs->GetFileList(path, &filelist, recursive);

	if (path.compare(".") == 0)
	{
		std::cout << "\nFiles in current directory :\n";
	}
	else
	{
		std::cout << "\nFiles in " << path << " :\n";
	}
	PIL::FileInfo_Vec::iterator it = filelist.begin();
	while (it != filelist.end())
	{
		std::cout << it->name << std::endl;
		it++;
	}

	if(fs)
		delete fs;
	return 0;
}
