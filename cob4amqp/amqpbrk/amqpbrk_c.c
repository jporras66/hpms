#include <string.h>
#include <stdio.h>
#include "amqpbrk.h"
#include "amqpbrk.hpp"
#include "common.h"
 
int16_t getNextValue  ( const char *input , const int16_t len, int16_t offset, char* dest )  ; 

int16_t amqpConnectCC ( const char *hostname, const char *port, const char *amqpversion, char *errstatus )
{
#ifdef DEBUG
    printf( "amqpConnectCC->Connecting to url : %s - %s \n" , hostname , port );
#endif
//
    memset ( errstatus, '\0', MAX_ERRSTATUS_LENGTH );
//
    int16_t length = computeLength ( hostname, MAX_HOSTNAME_LENGTH  );
    if ( length == -1) {
        std::stringstream ss ;
        ss << "amqpConnectCC.MAX_HOSTNAME_LENGTH up to " << MAX_HOSTNAME_LENGTH - 1 << " characters !! :-) !!" ;
        std::string s =  ss.str(); 
        strncpy( errstatus, s.c_str() , MAX_ERRSTATUS_LENGTH );
        return -1 ;
    }
//
    length = computeLength ( port, MAX_PORT_LENGTH  );
    if ( length == -1) {
        std::stringstream ss ;
        ss << "amqpConnectCC.MAX_PORT_LENGTH up to " << MAX_PORT_LENGTH - 1 << " characters !! :-) !!" ;
        std::string s =  ss.str(); 
        strncpy( errstatus, s.c_str() , MAX_ERRSTATUS_LENGTH );
        return -1 ;
    }
//
    length = computeLength ( amqpversion, MAX_AMQP_LENGTH  );
    if ( length == -1) {
        std::stringstream ss ;
        ss << "amqpConnectCC.MAX_AMQP_LENGTH up to " << MAX_AMQP_LENGTH - 1 << "characters !! :-) !!" ;
        std::string s =  ss.str(); 
        strncpy( errstatus, s.c_str() , MAX_ERRSTATUS_LENGTH );
        return -1 ;
    }
//
    std::string ihostname     ( hostname );
    std::string iport         ( port );
    std::string iamqpversion  ( amqpversion );
    std::string oerrstatus    ( MAX_ERRSTATUS_LENGTH, '\0' );
//
    int16_t rc = amqpConnect ( ihostname, iport, iamqpversion , oerrstatus );
    strncpy( errstatus, oerrstatus.c_str(), MAX_ERRSTATUS_LENGTH  ) ;
    if ( rc == -1) {
        return -1 ;
    }
    return 0;
}

int16_t amqpSendCC ( const char *queue, const char *msg, const int16_t msglen, const char *mimetype, char *errstatus ) {
//
#ifdef DEBUG
    printf( "amqpSendCC : %s -> %d -> 0x%x \n" , queue , msglen, msglen );
#endif
//
    memset ( errstatus, '\0', MAX_ERRSTATUS_LENGTH );
    int16_t rc = validateQueueDataLengthCC ( queue, msg, msglen, errstatus  );
    if ( rc == -1) {
        return -1 ;
    }
//
    std::string iqueue ( queue  );
    std::string imsg   ( msg , msglen );
//
/*
 * string_vector &properties (empty)
 */
    string_vector properties ;
//
    std::string oerrstatus   = "" ;
    std::string imimetype = mimetype ;
    Message message ;
    message.setContentType  ( imimetype );
    message.setContentObject( imsg );
//
    rc = amqpSendMessage ( iqueue, message, properties, oerrstatus );
    strncpy( errstatus, oerrstatus.c_str(), MAX_ERRSTATUS_LENGTH  ) ;
    if ( rc == -1) {
        return -1 ;
    }
//
    return 0;
}
//
int16_t amqpSendMessageCC ( const char *queue, const char *msg, const int16_t msglen, const char *mimetype, const PropertyList *p, char *errstatus ) {
//
#ifdef DEBUG
    printf( "amqpSendMessageCC : %s -> %d -> 0x%x \n" , queue , msglen, msglen );
#endif
//
    memset ( errstatus, '\0', MAX_ERRSTATUS_LENGTH );
    int16_t rc = validateQueueDataLengthCC ( queue, msg, msglen, errstatus  );
    if ( rc == -1) {
        return -1 ;
    }
    std::string iqueue ( queue  );
    std::string imsg   ( msg , msglen );
//
    std::string oerrstatus   = "" ;
    std::string imimetype = mimetype ;
//
    Message message ;
    message.setContentType  ( imimetype );
    message.setContentObject( imsg );
/*
 * string_vector &properties :
 *        Property format is as follows :    "<property_name>=<property_value>"
 *        Example                       :    "text-plain-message-id=64b99526-32c9-4e4b-95fd-84b2214841f5" 
 */
    string_vector properties ;
    std::string dummy = string ( p->arr );
    std::size_t start_position = 0 ; 
    for ( int k=0; k< p->num; k++){
        std::size_t end_position = dummy.find ( PROPERTY_SEPARATOR, start_position ) ;
        if ( end_position != std::string::npos ) {
            std::size_t len  = end_position - start_position ;
            std::string property = dummy.substr ( start_position, len ) ;
            properties.push_back ( property ) ;
            start_position = end_position + 1;
#ifdef DEBUG
        std::cout <<  "amqpSendMessageCC property -> " + property  << std::endl ;
#endif
        }
    }
/*
 *         amqpSendMessage ( const std::string &queue, Message &message, string_vector &properties, std::string &errstatus ) 
 */
#ifdef DEBUG
    std::cout <<  "amqpSendMessageCC imsg -> " + imsg  << std::endl ;
#endif
    rc = amqpSendMessage ( queue, message, properties, oerrstatus );
    strncpy( errstatus, oerrstatus.c_str(), MAX_ERRSTATUS_LENGTH  ) ;
    if ( rc == -1) {
        return -1 ;
    }
//
    return 0;
}
//
int16_t amqpRecvCC    ( const char *queue, char *msg , char *mimetype, char *errstatus  ) {
//
    char msgid [MAX_MESSAGE_ID_LENGTH];
    memset ( msgid , '\0', MAX_MESSAGE_ID_LENGTH ) ;
    PropertyList p ;
//
    int16_t rc = amqpRecvMessageCC ( queue, msgid, msg, mimetype, &p , errstatus  ) ;
    return rc ;
}
//

int16_t amqpRecvMessageCC ( const char *queue, char *msgid, char *msg, char *mimetype, PropertyList *p , char *errstatus  )  {
//
    memset ( msgid ,    '\0', MAX_MESSAGE_ID_LENGTH );
    memset ( msg ,      '\0', MAX_MESSAGE_LENGTH );
    memset ( mimetype , '\0', MAX_MIMETYPE_LENGTH );
    memset ( errstatus, '\0', MAX_ERRSTATUS_LENGTH );
//
    int16_t rc = computeLength ( queue, MAX_QUEUENAME_LENGTH  );
    if ( rc == -1) {
        strcpy( errstatus, "amqpRecvMessageCC.MAX_QUEUENAME_LENGTH up to 100 characters !! :-) !!" );
        return -1 ;
    }
    Message  message ;
    std::string iqueue     ( queue  ); 
    std::string oerrstatus ( MAX_ERRSTATUS_LENGTH, '\0' );
//
    rc = amqpRecvMessage ( iqueue , message, oerrstatus ) ;
    if ( rc == -1) {
        printf( "amqpRecvMessageCC.amqpRecvQueue %s %s : \n", iqueue.c_str(), oerrstatus.c_str() )  ;
        strncpy( errstatus, oerrstatus.c_str(), MAX_ERRSTATUS_LENGTH  ) ;
        return -1 ;
    }
//
    int k = 0 ;
    qpid::types::Variant::Map mp = message.getProperties();
    std::string properties = "";
    for ( qpid::types::Variant::Map::iterator it=mp.begin(); it !=mp.end() ; ++it ){
          std::string name  = it->first ;
          std::string value = it->second ;
          properties = properties + name + "=" + value + PROPERTY_SEPARATOR ;
#ifdef DEBUG 
          std::cout <<  "amqpRecvMessageCC Property is :  name/length  -> " << name << " " << name.length()  << std::endl ;
          std::cout <<  "                                 value        -> " << value << std::endl ;
#endif
          p->num = k+1 ;   
          k++ ;
    }
    //strncpy( p->arr, properties.c_str(), MAX_MESSAGE_PROPERTIES_LENGTH ) ;
    strncpy( p->arr, properties.c_str(), properties.length() ) ;
    if ( properties.length() > MAX_MESSAGE_PROPERTIES_LENGTH ) {
        std::cout <<  "WARNING - amqpRecvMessageCC.MAX_MESSAGE_PROPERTIES_LENGTH " << MAX_MESSAGE_PROPERTIES_LENGTH - 2 << " some characters has been truncated!! :-) !!" << std::endl ;  ;
    }
/*
 * Check if the message has content-type
 */
    std::string omimetype   = message.getContentType();
    strncpy ( mimetype, omimetype.c_str() , omimetype.length() );  
/*
 * Assign msgid and msg content 
 */
    std::string messageid   = message.getMessageId () ;
    strncpy( msgid, messageid.c_str() , messageid.length() );
//
    std::string response   ( MAX_MESSAGE_LENGTH, '\0' );
    response   = message.getContent() ;
    int16_t length = response.length() ;  
//
    if ( length > MAX_MESSAGE_LENGTH ) {
        std::cout <<  "WARNING - amqpRecvMessageCC.MAX_MESSAGE_LENGTH up to " << MAX_MESSAGE_LENGTH << " some characters has been truncated!! :-) !!" << std::endl ;  ;
        strncpy ( msg, response.c_str() , MAX_MESSAGE_LENGTH );
    }else{
        strncpy ( msg,response.c_str(), length ) ;
    }
//
    return length ;
}

int16_t getPropertyValueCC  ( const PropertyList *p, const char *name_c, char *value_c  ) {

   int num = p->num ;
   std::string dummy = string( p->arr) ;
   std::string name  = string ( name_c ) ;
   size_t start  = dummy.find( name, 0 ) ;
/*
 * Not found
 */ 
   if ( start == std::string::npos ) return -1 ; 
//
   size_t end = dummy.find ( PROPERTY_SEPARATOR, start ) ;
   if ( end == std::string::npos ) return -1 ; 
// 
   std::string property  =  dummy.substr( start, end - start ); 
   std::string value     = string ( value_c ) ;
   bool b = nameval ( property, name , value ) ;
   if ( ! b ) return -1 ;
//
   strncpy ( value_c, value.c_str(), value.length() );
   return value.length() ;
}
int16_t validateQueueDataLengthCC ( const char *queue, const char *msg,  const int16_t msglen, char *errstatus ) {

    int16_t rc = computeLength ( queue, MAX_QUEUENAME_LENGTH  );
    if ( rc == -1) {
        std::stringstream ss ;
        ss << "validateQueueDataLengthCC.MAX_QUEUENAME_LENGTH up to " << MAX_QUEUENAME_LENGTH - 1 << " characters !! :-) !!" ;
        std::string s =  ss.str(); 
        strncpy( errstatus, s.c_str() , MAX_ERRSTATUS_LENGTH );
        return -1 ;
    }

    if ( msglen < 0 || msglen > MAX_MESSAGE_LENGTH ){
        std::stringstream ss ;
        ss << "validateQueueDataLengthCC.MAX_MESSAGE_LENGTH up to " << MAX_MESSAGE_LENGTH - 1 << "characters !! :-) !!" ;
        std::string s =  ss.str(); 
        strncpy( errstatus, s.c_str() , MAX_ERRSTATUS_LENGTH );
        return -1 ;
    }
//
    return 0 ;
}
//
