#ifndef MESSAGE_H
#define MESSAGE_H

#include <string.h>
#include <stdint.h>
#include <sys/types.h>
#include <unistd.h>

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
//
const int16_t  MAX_BROKER_LENGTH     =  50+1  ;
const int16_t  MAX_QUEUENAME_LENGTH  = 100+1  ;
const int16_t  MAX_FILENAME_LENGTH   =  50+1  ;
const int16_t  MAX_PATH_LENGTH       = 255+1  ;
const int16_t  MAX_HOSTNAME_LENGTH   =  50+1  ;
const int16_t  MAX_PORT_LENGTH       =   5+1  ;
const int16_t  MAX_ERRSTATUS_LENGTH  = 100+1  ;
const int16_t  MAX_MIMETYPE_LENGTH   =  50+1  ;
const int16_t  MAX_DBNAME_LENGTH     =  50+1  ;
const int16_t  MAX_DBCONSTR_LENGTH   = 100+1  ;
const int16_t  MAX_PMG_LENGTH	     =  30+1  ;
const int16_t  MAX_NUM_THREADS       =   300  ;
//
const int16_t  MAX_MESSAGE_ID_LENGTH = 100+1  ;
const int16_t  MAX_MESSAGE_LENGTH    = 4096   ;
const int16_t  MAX_AMQP_LENGTH 	     = 8+1    ;
const char     DEFAULT_AMQP_VERSION[]= "amqp1.0" ;  
/*
 * Start of Message properties  - C Style Struct definition  
 * 
 */
const int16_t  MAX_MESSAGE_PROPERTIES_LENGTH = 1026  ;
const char     PROPERTY_SEPARATOR[] = " " ;
const char     APL_MESSAGE_TYPE[]   = "apl_message_type" ;
const char     APL_MESSAGE_UUID[]   = "messageUUID" ;
 
typedef struct // 2+1024 =1026 bytes
{
    int16_t  num ;
    char     arr[1024];

} PropertyList;
//
/*
 * End of Message properties  - C Style Struct definition  
 */
#ifdef __cplusplus
}
#endif
//
#endif  /* ! MESSAGE_H */
