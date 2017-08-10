#include <stdio.h>
#include <time.h>
#include <sys/time.h>
#include <sys/types.h>
#include <stdint.h>
#include <unistd.h>
#include "sleep.h"
/*
    Puts this process to sleep for an amount of time determined by usec (in microseconds)
*/
int16_t micro_sleep ( long usec ){   
//
    char time_string[100];
    struct timeval  tv;
    struct tm*      ptm;
//
#ifdef DEBUG
        printf("microsleep starts for %ld microseconds \n", usec);
//
// Obtain the time of day, and convert it to a tm struct.
//
        gettimeofday (&tv, NULL);
        ptm = localtime (&tv.tv_sec);
        strftime (time_string, sizeof (time_string), "%Y-%m-%d %H:%M:%S", ptm);
        printf( "%s.%06ld \n", time_string, tv.tv_usec);
#endif
    
    usleep( usec );

#ifdef DEBUG
        gettimeofday (&tv, NULL);
        ptm = localtime (&tv.tv_sec);
        strftime (time_string, sizeof (time_string), "%Y-%m-%d %H:%M:%S", ptm);
        printf( "%s.%06ld \n", time_string, tv.tv_usec);
        printf("microsleep stops \n");
#endif     
//
    return 0;
} ;
/*
    Puts this process to sleep for an amount of time determined in seconds
*/
int16_t to_sleep ( int16_t sec ){   
//
    long usec = ( (long) sec ) * 1000000L;
    micro_sleep ( usec );
    return 0;
} ;


