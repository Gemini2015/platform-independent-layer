#ifndef _PIL_TIME_H_
#define _PIL_TIME_H_

#include "PILPrerequisites.h"

namespace PIL
{

	class PILTimer
	{
	public:
		PILTimer();
		~PILTimer();

		void Reset();
		uint32 GetMillisecondTime();

	protected:

#if defined(PLATFORM_WIN32)

		LARGE_INTEGER m_StartTime;
		DWORD m_StartTick;
		LONGLONG m_LastTime;

#elif defined(PLATFORM_LINUX) 


#endif

	};

}

#endif