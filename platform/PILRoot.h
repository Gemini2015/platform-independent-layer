#ifndef _PIL_ROOT_H_
#define _PIL_ROOT_H_

#include "PILInclude.h"

namespace PIL
{

	class Root
	{
	public:
		static Root *Instance;
	public:
		Root()
		{
			mWindowManger = new WindowManager();

			mFileSystem = new FileSystem();

			mTimer = new Timer();

			Instance = this;
		}
		~Root()
		{
			if (mWindowManger)
				delete mWindowManger;
			if (mFileSystem)
				delete mFileSystem;
			if (mTimer)
				delete mTimer;

			Instance = NULL;
		}

		WindowManager* GetWindowManger() const
		{
			return mWindowManger;
		}

		FileSystem* GetFileSystem() const
		{
			return mFileSystem;
		}

		Timer* GetTimer() const
		{
			return mTimer;
		}

		static Root* SingletonPtr() const
		{
			return Instance;
		}

		static Root& Singleton() const
		{
			assert(Instance != NULL);
			return *Instance;
		}

	private:
		WindowManager *mWindowManger;
		FileSystem *mFileSystem;
		Timer *mTimer;
	};

	Root * Root::Instance = NULL;
}

#endif