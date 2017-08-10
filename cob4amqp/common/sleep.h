#ifndef SLEEP__H
#define SLEEP__H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

int16_t micro_sleep ( long usec );  
int16_t to_sleep ( int16_t sec );  

#ifdef __cplusplus
}
#endif

#endif /* SLEEP__H */

