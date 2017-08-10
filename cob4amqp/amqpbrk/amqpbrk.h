#ifndef AMQPBRK_H
#define AMQPBRK_H

#include <stdint.h>
#include "message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */
/*
 * Start of C wrappers functions
 */
int16_t amqpConnectCC 		( const char *hostname, const char *port, const char *amqpversion, char *errstatus ) ;
/*
 * amqpSendCC   : Send message to a Queue 
 *                Input values  :  queue 		queue name 
 *                Output values :  msg  		message
 *                                 msglen  	message length  
 *                                 mimetype	mimetype "text/plain" or "application/octet-stream"  
 *                                 errstatus	error status message (if any) 
 *
 * Returns      : 0 (ok) -1 (error, check errstatus )
 *
 */
int16_t amqpSendCC ( const char *queue, const char *msg, const int16_t msglen, const char *mimetype, char *errstatus ) ;
int16_t amqpSendMessageCC ( const char *queue, const char *msg, const int16_t msglen, const char *mimetype, const PropertyList *propertylist, char *errstatus ) ;
/*
 * amqpRecvCC   : receive message from a Queue 
 *                Input values  :  queue 		queue name 
 *                Output values :  msg  		message
 *                                 mimetype	mimetype "text/plain" or "application/octet-stream"  
 *                                 errstatus	error status message (if any) 
 *
 * amqpRecvMessageCC : receive message from a Queue with message properties. Same parameters as above.
 *                     Output values :  msgid is messageId for the dequed message
 *                                      PropertyArr	Array of properties name/value (up to 40) (see message.h)
 *
 * Returns number bytes received or -1 (error)
 *
 */
int16_t amqpRecvCC ( const char *queue, char *msg , char *mimetype, char *errstatus  ) ;
int16_t amqpRecvMessageCC   ( const char *queue, char *msgid, char *msg, char *mimetype, PropertyList *propertylist, char *errstatus  ) ;
int16_t getPropertyValueCC  ( const PropertyList *propertylist, const char *property, char *value  ) ;
//
/*
 * END of C wrappers functions
 */
int16_t validateQueueDataLengthCC 	( const char *queue, const char *msg, const int16_t msglen, char *errstatus ) ;
//
//
#ifdef __cplusplus
}
#endif
//
#endif  /* ! AMQPBRK_H */
