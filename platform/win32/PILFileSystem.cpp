#include "PILFileSystem.h"

namespace PIL
{
	HRESULT FileSystem::CreateDir(std::string path)
	{
		if (path.empty())
			return E_INVALIDARG;
		HRESULT hr;
		if (_mkdir(path.c_str()) == 0)
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
		if (_rmdir(path.c_str()) == 0)
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
		/*if (path.empty())
			return E_INVALIDARG;*/
		if (filelist == NULL)
			return E_INVALIDARG;
		

		struct _finddata_t filedata;
		intptr_t hFile;
		FileInfo info;
		FilePath_Vec pathlist;
		
		std::string fpath = RelativeToAbsPath(path);
		if (!IsFileOrDirExist(fpath))
			return E_INVALIDARG;
		if (fpath.empty())
			return E_FAIL;
		pathlist.push_back(fpath);
		

		while (!pathlist.empty())
		{
			std::string tpath = pathlist.front();
			pathlist.erase(pathlist.begin());

			if(_chdir(tpath.c_str()) != 0)
				continue;

			hFile = _findfirst(filter.c_str(), &filedata);
			if (hFile == -1)
			{
				continue;
			}
			
			do 
			{
				if(strcmp(filedata.name, ".") == 0 || strcmp(filedata.name, "..") == 0)
					continue;

				info.name.assign(filedata.name);
				info.size = filedata.size;
				info.attrib = filedata.attrib;
				info.time_write = filedata.time_write;

				filelist->push_back(info);

				if (recursive && filedata.attrib & _A_SUBDIR)
				{
					fpath = RelativeToAbsPath(std::string(filedata.name));
					if (!fpath.empty())
					{
						pathlist.push_back(std::string(fpath));
					}
				}

			} while (_findnext(hFile, &filedata) == 0);
			
			_findclose(hFile);
		}

		return S_OK;
	}

}
