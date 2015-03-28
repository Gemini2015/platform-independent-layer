#include "PILTime.h"
#include <algorithm>

namespace PIL
{

	Timer::Timer()
	{
		Reset();
	}

	Timer::~Timer()
	{

	}

	void Timer::Reset()
	{
		QueryPerformanceCounter(&m_StartTime);
		m_StartTick = GetTickCount();

		m_LastTime = 0;
	}

	uint32 Timer::GetMillisecondTime()
	{
		LARGE_INTEGER frequency;
		QueryPerformanceFrequency(&frequency);

		LARGE_INTEGER endTime;
		QueryPerformanceCounter(&endTime);

		LONGLONG TimeOffset = endTime.QuadPart - m_StartTime.QuadPart;

		unsigned long Ticks = (unsigned long)(1000 * TimeOffset / frequency.QuadPart);

		unsigned long check = GetTickCount() - m_StartTick;
		signed long msecOff = (signed long)(Ticks - check);
		if (msecOff < -100 || msecOff > 100)
		{
			LONGLONG adjust = (std::min)(msecOff * frequency.QuadPart / 1000, TimeOffset - m_LastTime);
			m_StartTime.QuadPart += adjust;
			TimeOffset -= adjust;

			Ticks = (unsigned long)(1000 * TimeOffset / frequency.QuadPart);
		}

		m_LastTime = TimeOffset;

		return Ticks;
	}

}