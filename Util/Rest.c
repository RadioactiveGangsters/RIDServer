#include "Rest.h"

void Rest(int s)
{
	#ifdef _WIN32
		Sleep(s*1000);
	#else
		sleep(s);
	#endif
}
