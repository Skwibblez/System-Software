#include "utils.h"


long get_elasped_time(struct timeval& start, struct timeval& end)
{

	long mtime = 0;
    long seconds  = end.tv_sec  - start.tv_sec;
    long useconds = end.tv_usec - start.tv_usec;
	///
	//std::cerr << "Useconds is: " << useconds << std::cerr;
	//std::cerr << "Seconds is: " << seconds << std::cerr;
	///
    mtime = ((seconds) * 1000 + useconds/1000.0) + 0.5;

	return mtime;
}



