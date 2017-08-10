#ifndef COB4BRK_H
#define COB4BRK_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/*
 * C style
 */
int16_t cob_init( int argc, char **argv ) ;
//int16_t driver	( const char *dbconf, const char *queueconf, char *errstatus );
int16_t driver	( const char *db_constr, const char *db_user, const char *db_password, const char *queueconf, char *errstatus );
int16_t timer	( const char *db_name, const char *db_user, const char *db_password, const char *msg, 
                  const char *mimetype, int16_t* pmsglen, int16_t* ptimeout_seconds, char *errstatus );
//
#ifdef __cplusplus
}
#endif
//
#endif  /* ! COB4BRK_H */
