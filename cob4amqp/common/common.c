#include <unistd.h>
#include <sys/stat.h>
#include <stdio.h>
#include <string.h>
#include "common.h"

int16_t computeLength  ( const char *txt , const int16_t limit) {

    const char *p = txt ;
    int16_t length=0;
    int k=0;
    for ( k=0; k<limit; k++){
        if ( *(p+length) != '\0' ) {
            length++;
        }else{
            return length ;
        }
    }
    return -1 ;
}

int16_t  file_exist (const char *filename) {

      struct stat   buffer;
      return (stat (filename, &buffer) == 0);
}

int16_t display_str 	( const char *str ) {
    int16_t len=computeLength( str, 1024 ) ;
    char dummy [len] ; 
    strncpy ( dummy, str, len );
    printf("     %s \n",dummy); 
}
