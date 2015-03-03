#include "PILTime.h"
#include <algorithm>

namespace PIL
{

	PILTimer::PILTimer()
	{
		Reset();
	}

	PILTimer::~PILTimer()
	{

	}

	void PILTimer::Reset()
	{
		gettimeofday(&m_StartTime, NULL);
	}

	uint32 PILTimer::GetMillisecondTime()
	{
		timeval endTime;
		gettimeofday(&endTime, NULL);

		// timeval 由 tv_sec(秒)， tv_usec(微秒) 共同组成
		unsigned long elapsedTime = (endTime.tv_sec - m_StartTime.tv_sec) * 1000;
		elapsedTime += (endTime.tv_usec - m_StartTime.tv_usec) / 1000;

		return elapsedTime;
	}

}
