#ifndef AMQPBRK_HPP
#define AMQPBRK_HPP

#include <qpid/messaging/Connection.h>
#include <qpid/messaging/Session.h>
#include <qpid/messaging/Sender.h>
#include <qpid/messaging/Receiver.h>
#include <qpid/messaging/Message.h>
#include <qpid/types/Variant.h>
#include <cstdlib>
#include <string>
#include <sstream>
#include <cassert>
#include <iostream>
#include <map>
#include <vector>
#include "message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

using namespace qpid::messaging;
using namespace std;
typedef std::vector<std::string> string_vector;
 /*
 * C++ wrappers functions
 */
int16_t amqpConnect     ( const std::string &hostname, const std::string &port, const std::string &amqpversion, std::string &errstatus, const std::string &username = "admin", const std::string &password = "admin" ) ;
int16_t amqpSetSender   ( const std::string &queue, Sender   &sender,   std::string &errstatus ) ;
int16_t amqpSetReceiver ( const std::string &queue, Receiver &receiver, std::string &errstatus ) ;
int16_t amqpSetBrowser  ( const std::string &queue, Receiver &receiver, std::string &errstatus ) ;
Session amqpGetSession  ( ) ;
/*
 * mimetype :  "text/plain" ,  "application/octet-stream" 
 */
std::string  checkAMQP2JMSmessageType  ( const Message &message,  const std::string &mimetype, std::string &messagetype, std::string &content_type ) ;
/*
 * string_vector &properties :
 *        Property format is as follows :    "<property_name>=<property_value>"
 *        Example                       :    "messageid=64b99526-32c9-4e4b-95fd-84b2214841f5" 
 */
int16_t amqpSendMessage   ( const std::string &queue, Message &message, string_vector &properties, std::string &errstatus ) ;
int16_t amqpRecvMessage   ( const std::string &queue, Message &message, std::string &errstatus) ;
int16_t amqpBrowseMessage ( const std::string &queue, Message &message, std::string &errstatus) ;
int16_t amqpBye () ;
//
void bulkMessage ( Message &message ) ;
void setEntries( const string_vector &properties, qpid::types::Variant::Map &map ) ; 
bool nameval(const std::string& in, std::string& name, std::string& value) ; 
void setProperties( const string_vector &properties, Message& message ) ;
void setProperty( Message& message, const std::string& property ) ;
//
#ifdef __cplusplus
}
#endif
//
#endif  /* ! AMQPBRK_HPP */
