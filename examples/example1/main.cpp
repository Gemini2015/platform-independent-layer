#include "PILPrerequisites.h"
#include <stdio.h>

int main(int argc, char** argv)
{
#if defined(PLATFORM_WIN32)
	printf("Platform: Win32\n");
#elif defined(PLATFORM_LINUX)
	printf("Platform: Linux\n");
#endif

	printf("\nData type size:\n");
	printf("int8:\t\t%d Byte\n", sizeof(int8));
	printf("int16:\t\t%d Byte\n", sizeof(int16));
	printf("int32:\t\t%d Byte\n", sizeof(int32));
	printf("int64:\t\t%d Byte\n", sizeof(int64));
	printf("float:\t\t%d Byte\n", sizeof(float));
	printf("double:\t\t%d Byte\n", sizeof(double));
	printf("void*:\t\t%d Byte\n", sizeof(void*));

	uint32 val = 0x01020304;
	uint8* p = (uint8*)&val;
	printf("\nValue %d (Dec) = 0x%X (Hex)\n", val, val);
	printf("Byte Order[1 2 3 4]: 0x%X 0x%X 0x%X 0x%X\n", *(p), *(p + 1), *(p + 2), *(p + 3));
	if (*p == 0x01)
	{
		printf("Big Endian\n");
	}
	else if (*p == 0x04)
	{
		printf("Little Endian\n");
	}
}