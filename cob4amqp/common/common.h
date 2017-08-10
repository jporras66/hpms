#ifndef COMMON_H
#define COMMON_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/*
 * C style
 */
int16_t computeLength   ( const char *txt , const int16_t limit )  ;
int16_t file_exist 	( const char *filename ) ;
int16_t display_str 	( const char *str ) ;
//
#ifdef __cplusplus
}
#endif
//
#endif  /* ! COMMON_H */
