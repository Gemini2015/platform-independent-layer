#include "PILFileSystem.h"
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <dirent.h>
#include <string.h>

namespace PIL
{
	HRESULT FileSystem::CreateDir(std::string path)
	{
		if (path.empty())
			return E_INVALIDARG;
		HRESULT hr;
		if (mkdir(path.c_str(), S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH) == 0)
		{
			hr = S_OK;
		}
		else hr = E_FAIL;
		return hr;
	}

	HRESULT FileSystem::RemoveDir(std::string path)
	{
		if (path.empty())
			return E_INVALIDARG;
		HRESULT hr;
		if (rmdir(path.c_str()) == 0)
		{
			hr = S_OK;
		}
		else hr = E_FAIL;
		return hr;
	}

	HRESULT FileSystem::RenameDir(std::string oldname, std::string newname)
	{
		if (oldname.empty() || newname.empty())
			return E_INVALIDARG;
		HRESULT hr;
		if (rename(oldname.c_str(), newname.c_str()) == 0)
		{
			hr = S_OK;
		}
		else hr = E_FAIL;
		return hr;
	}

	HRESULT FileSystem::GetFileList(const std::string path, FileInfo_Vec *filelist, bool recursive /*= false*/, const std::string filter /*= "*.*"*/)
	{
		if (filelist == NULL)
			return E_INVALIDARG;
		
		DIR *dir = NULL;
		struct dirent* pDirent = NULL;
		FileInfo info;
		FilePath_Vec pathlist;
		
		std::string fpath = RelativeToAbsPath(path);
		if(!IsFileOrDirExist(fpath))
			return E_INVALIDARG;
		if(fpath.empty())
			return E_FAIL;
		pathlist.push_back(fpath);

		while(!pathlist.empty())
		{
			std::string tpath = pathlist.front();
			pathlist.erase(pathlist.begin());

			dir = opendir(tpath.c_str());
			if(dir == NULL)
				continue;

			while((pDirent = readdir(dir)) != NULL)
			{
				if(strcmp(pDirent->d_name, ".") == 0 || strcmp(pDirent->d_name, "..") == 0)
					continue;

				info.name.assign(pDirent->d_name);
				info.size = pDirent->d_reclen;
				info.attrib = pDirent->d_type;

				filelist->push_back(info);

				if(recursive && pDirent->d_type == DT_DIR)
				{
					fpath = RelativeToAbsPath(std::string(pDirent->d_name));
					if(!fpath.empty())
					{
						pathlist.push_back(std::string(fpath));
					}
				}

			}

			closedir(dir);

		}

		return S_OK;
	}

}
