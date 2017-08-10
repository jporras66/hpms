#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include "common.h"

int main(int argc, char *argv[]) {
    char time_string[100];
    struct timeval  tv;
    struct tm*      ptm; 

	printf("sleep for 2.000.000 microsenconds (2 secs)  \n");
//
// Obtain the time of day, and convert it to a tm struct.
//
	gettimeofday (&tv, NULL); 
	ptm = localtime (&tv.tv_sec);
	strftime (time_string, sizeof (time_string), "%Y-%m-%d %H:%M:%S", ptm);
	printf( "%s.%06ld \n", time_string, tv.tv_usec);
//	
	micro_sleep ( 2000000L );
//
	gettimeofday (&tv, NULL);
	ptm = localtime (&tv.tv_sec);
	strftime (time_string, sizeof (time_string), "%Y-%m-%d %H:%M:%S", ptm);
	printf( "%s.%06ld \n", time_string, tv.tv_usec);
//
	printf("sleep stops \n");
//	
    return 0;
}

