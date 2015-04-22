#include "PILInclude.h"
#include <iostream>

#if defined(PLATFORM_WIN32)

void sleep(uint32 second)
{
	Sleep(second * 1000);
}

#elif

#include <unistd.h>

#endif

int main()
{
	PIL::Root* root = new PIL::Root();
	PIL::Timer* tm = root->GetTimer();
	std::cout << "Current Time " << tm->GetTimeString(PIL::TimeStringFormat::YMDHS) << std::endl;
	uint32 ms = tm->GetMillisecondTime();
	std::cout << "Current Millisecond " << ms << std::endl;

	std::cout << "Sleep 5000 ms..." << std::endl;

	sleep(5);

	std::cout << "Current Time " << tm->GetTimeString(PIL::TimeStringFormat::YMDHS) << std::endl;
	uint32 msEnd = tm->GetMillisecondTime();
	uint32 offset = msEnd - ms;
	std::cout << "Current Millisecond " << msEnd << std::endl;
	std::cout << "Millisecond offset " << offset << std::endl;

	std::cout << std::endl;
	// H_S
	std::string timetext = tm->GetTimeString(PIL::TimeStringFormat::H_S);
	std::cout << "H_S\t\t" << timetext << std::endl;
	// HS
	timetext = tm->GetTimeString(PIL::TimeStringFormat::HS);
	std::cout << "HS\t\t" << timetext << std::endl;
	// Y_M_D
	timetext = tm->GetTimeString(PIL::TimeStringFormat::Y_M_D);
	std::cout << "Y_M_D\t\t" << timetext << std::endl;
	// Y_M_D_H_S
	timetext = tm->GetTimeString(PIL::TimeStringFormat::Y_M_D_H_S);
	std::cout << "Y_M_D_H_S\t" << timetext << std::endl;
	// YMD
	timetext = tm->GetTimeString(PIL::TimeStringFormat::YMD);
	std::cout << "YMD\t\t" << timetext << std::endl;
	// YMDHS
	timetext = tm->GetTimeString(PIL::TimeStringFormat::YMDHS);
	std::cout << "YMDHS\t\t" << timetext << std::endl;
	// YMDHS_FILE
	timetext = tm->GetTimeString(PIL::TimeStringFormat::YMDHS_FILE);
	std::cout << "YMDHS_FILE\t" << timetext << std::endl;


	struct tm t_time;
	t_time.tm_year = 2015 - 1900;
	t_time.tm_mon = 4;
	t_time.tm_mday = 19;
	t_time.tm_hour = 9;
	t_time.tm_min = 5;
	t_time.tm_sec = 15;
	timetext = tm->GetTimeString(PIL::TimeStringFormat::YMDHS, &t_time);
	std::cout << "YMDHS\t\t" << timetext << std::endl;

	getchar();
	return 0;
}