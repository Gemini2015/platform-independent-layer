#include "PILTime.h"
#include <algorithm>
#include <time.h>

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


	std::string Timer::GetTimeString(TimeStringFormat fmt, struct tm* t /* = nullptr */)
	{
		struct tm* pt = t;
		if (pt == nullptr)
		{
			time_t curtimes;
			time(&curtimes);
			tm t_temp;
			localtime_s(&t_temp, &curtimes);
			pt = &t_temp;
		}

		if (time == NULL)
			return std::string("");
		char buf[260] = { 0 };
		switch (fmt)
		{
		case TimeStringFormat::YMD:
			strftime(buf, 260, "%Y-%m-%d", pt);
			break;
		case TimeStringFormat::Y_M_D:
			strftime(buf, 260, "%Y_%m_%d", pt);
			break;
		case TimeStringFormat::YMDHS:
			strftime(buf, 260, "%Y-%m-%d %H:%M:%S", pt);
			break;
		case TimeStringFormat::YMDHS_FILE:
			strftime(buf, 260, "%Y-%m-%d-%H-%M-%S", pt);
			break;
		case TimeStringFormat::Y_M_D_H_S:
			strftime(buf, 260, "%Y_%m_%d_%H_%M_%S", pt);
			break;
		case TimeStringFormat::HS:
			strftime(buf, 260, "%H:%M:%S", pt);
			break;
		case TimeStringFormat::H_S:
			strftime(buf, 260, "%H_%M_%S", pt);
			break;
		default:
			strftime(buf, 260, "%Y-%m-%d", pt);
			break;
		}
		return std::string(buf);
	}

}