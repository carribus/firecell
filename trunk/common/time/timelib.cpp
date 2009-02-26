#ifdef _WIN32
  #include <windows.h>
#else
  #include <time.h>
  #include <sys/time.h>
#endif//_WIN32
#include "timelib.h"

FCULONG tl_getTime()
{
#ifdef _WIN32
  return (FCULONG)GetTickCount();
#else
	timeval tv;
	gettimeofday(&tv, 0);
	return (FCULONG)(tv.tv_sec * 1000) + (tv.tv_usec / 1000);
#endif//_WIN32
}