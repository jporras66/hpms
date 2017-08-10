#include "amqpbrk.hpp"

namespace amqpbrk
{
    Connection  connection ;
    Session     session ;
    std::map<std::string,Sender>    sendermap ;
    std::map<std::string,Receiver>  receivermap ;
    std::map<std::string,Receiver>  browsermap ;
}
using namespace amqpbrk;

/* Taken from Martin Stettner :
 * http://stackoverflow.com/users/81424/martinstettner
 */ 
struct HexCharStruct
{
  unsigned char c;
  HexCharStruct(unsigned char _c) : c(_c) { }
};

inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
{
  return (o << std::hex << (int)hs.c);
}

inline HexCharStruct hex(unsigned char _c)
{
  return HexCharStruct(_c);
}
/* END
 */ 
int16_t amqpConnect ( const std::string &hostname, const std::string &port, const std::string &amqpversion, std::string &errstatus, const std::string &username, const std::string &password ) {
//
    std::string url = "amqp:tcp:"+hostname+":"+port;
    std::string amqpversionTmp = amqpversion;
    if ( amqpversionTmp != "amqp1.0" && amqpversionTmp != "amqp0-10" ) {
         amqpversionTmp =  DEFAULT_AMQP_VERSION ;
         std::cout <<  "amqpConnect version must be : amqp1.0 or amqp0-10 " << std::endl ;
         std::cout <<  "amqpConnect selected by default : " + amqpversionTmp << std::endl ;
    }
    std::string connectionOptions = "{ protocol: " + amqpversionTmp +" }"  ;
    Connection connection  ( url, connectionOptions );
    connection.setOption("username", username);
    connection.setOption("password", password);
    connection.setOption("reconnect", true);
/*
 * When AMQP 1.0 is used, the following options apply:
 * The delivery- and message- annotations sections can be made available via nested maps with key x-amqp-delivery-annotations and
 * x-amqp-message-annotations respectively by specifying the 'nest_annotations' connection option. 
 * Otherwise they will simply be included in the properties.
 * 
 * 
    if ( amqpversionTmp == "amqp1.0") {
    connection.setOption("nest-annotations", true);
    }
 */
    try {
       amqpbrk::connection.open();
       //session = connection.createSession( );
       amqpbrk::session = amqpGetSession() ;
#ifdef DEBUG
        std::cout <<  "amqpConnect->connection OK to : " + url << std::endl;
#endif
    } catch(const std::exception& error) {
       std::cout <<  "amqpConnect->connection KO to " + url  + " -> " + error.what() << std::endl;
       amqpbrk::connection.close();
       return -1 ;
  }
  return 0;
}
//
Session amqpGetSession ( ) {
    bool b = amqpbrk::connection.isOpen();
    //std::cout <<  "connection.isOpen " << b << std::endl;
    Session s = amqpbrk::connection.createSession( );
    return s ;
}
//
int16_t amqpSetSender ( const string &queue,  Sender &sender, string &errstatus ) {
    std::map<std::string,Sender>::iterator it;
    it = sendermap.find( queue );
    if ( it == sendermap.end() ) {
// {create: always} then the queue will be created if it does not already
        std::string dummy = queue + " ; {create: always} ";
        //sender = session.createSender( queue );
        sender = session.createSender( dummy );
        sendermap.insert( std::pair< string,Sender > ( queue, sender) );
#ifdef DEBUG
    std::cout <<  "amqpSetSender->inserting queue destination : " + queue << std::endl ;
#endif
    }else{
        sender = sendermap.at( queue );
    }
    return 0;
}
//
int16_t amqpSetReceiver ( const string &queue, Receiver &receiver, string &errstatus ) { 

    std::map<std::string,Receiver>::iterator it;
    it = receivermap.find( queue );
    if ( it == receivermap.end() ) {
// {create: always} then the queue will be created if it does not already        
        std::string dummy = queue + " ; {create: always} ";
        //receiver = session.createReceiver( queue );
        receiver = session.createReceiver( dummy );
        receivermap.insert( std::pair< string,Receiver > ( queue, receiver) );
#ifdef DEBUG
    std::cout <<  "amqpSetReceiver->inserting queue destination : " + queue << std::endl ;
#endif
    }else{
        receiver = receivermap.at( queue );
    }
    return 0 ;
}
//
int16_t amqpSetBrowser ( const string &queue, Receiver &receiver, string &errstatus ) { 

    std::map<std::string,Receiver>::iterator it;
    it = browsermap.find( queue );
    if ( it == browsermap.end() ) {
// {create: always} then the queue will be created if it does not already        
        std::string dummy = queue + " ; {create: always, mode:browse} ";
        //receiver = session.createReceiver( queue );
        receiver = session.createReceiver( dummy );
        browsermap.insert( std::pair< string,Receiver > ( queue, receiver) );
#ifdef DEBUG
    std::cout <<  "amqpSetBrowser->inserting queue destination : " + queue << std::endl ;
#endif
    }else{
        receiver = browsermap.at( queue );
    }
    return 0 ;
}
//
int16_t  amqpSendMessage ( const string &queue, Message &message, string_vector &properties, string &errstatus )  {
//
    Sender sender ;
    int16_t rc = amqpSetSender ( queue, sender, errstatus ) ;
    if ( rc == -1) {
        errstatus = "amqpSendMessage.amqpSetSender :" + errstatus ;
        return -1 ;
    }
//
    setProperties( properties, message ) ;
/* 
 *  JMS AMQP QPID client needs to know the message type:
 *
 *                               Value              Mimetype
 *                               ===========        =====================================
 *  value ( x-opt-jms-msg-type ) 5   TextMessage    ""
 *                               3   BytesMessage   "application/octet-stream"
 *                               2   MapMessage     ""
 *                               4   StreamMessage  ""
 *                               1   ObjectMessage  "application/x-java-serialized-object"
 *                               0   Message        ""
 *
 * Check if the message has content-type
 */
    std::string mimetype   = message.getContentType();
/*
 * Try to assign a AMQP2JMS message type checking x-opt-jms-msg-type in message annotations
 * and set the custom property APL_MESSAGE_TYPE
 *
 */
    std::string messagetype ;
    std::string value; 
    std::string content_type; 
    value = checkAMQP2JMSmessageType ( message, mimetype, messagetype, content_type ) ;
    //message.setProperty( APL_MESSAGE_TYPE, messagetype );
    message.setProperty( APL_MESSAGE_TYPE, value );
//
#ifdef DEBUG
    bulkMessage ( message ) ;
    {
        std::string pname;
        std::string pvalue;
        std::cout <<  "amqpSendMessage->sending to queue : " << queue << std::endl ;
        for ( string_vector::iterator it = properties.begin() ; it != properties.end(); ++it ){ 
            nameval( *it, pname, pvalue) ;
            std::cout <<  "Property is : name  -> " << pname  << std::endl ;
            std::cout <<  "              value -> " << pvalue << std::endl ;
        }  
    }               
#endif
    sender.send(message);
    return 0;
}
//
int16_t  amqpRecvMessage ( const string &queue, Message &message,  string &errstatus  ) {

    Receiver receiver ;
    int16_t rc = amqpSetReceiver ( queue, receiver, errstatus ) ;
    if ( rc == -1) {
        errstatus = "amqpRecvMessage :" + errstatus ;
        return -1 ;
    }
    message = receiver.fetch() ;
    session.acknowledge( message ); 
//
#ifdef DEBUG
    bulkMessage ( message ) ;
    //std::cout <<  "amqpRecvMessage->after receiver.fetch() : " + queue + "->" + message.getContent() << "->mime : " + message.getContentType()  << std::endl ;
#endif
    return message.getContentSize() ;
}
//
int16_t  amqpBrowseMessage ( const string &queue, Message &message,  string &errstatus  ) {

    Receiver receiver ;
/*
 * New receiver in Browse mode
 */ 
    int16_t rc = amqpSetBrowser ( queue, receiver, errstatus ) ;
    if ( rc == -1) {
        errstatus = "amqpBrowseMessage :" + errstatus ;
        return -1 ;
    }
    message = receiver.fetch() ;
//
#ifdef DEBUG
    bulkMessage ( message ) ;
    //std::cout <<  "amqpBrowseMessage->after receiver.fetch() : " + queue + "->" + message.getContent() << "->mime : " + message.getContentType()  << std::endl ;
#endif
    return 0 ;
}
/*
 * 
 * http://docs.oasis-open.org/amqp/core/v1.0/os/amqp-core-messaging-v1.0-os.html#type-properties
 * Also see doc : Red Hat JBoss A-MQ 7.0 Using the AMQP JMS Client
 * 
 */
std::string  checkAMQP2JMSmessageType  ( const Message &message,  const std::string &mimetype, std::string &messagetype, std::string &content_type ) {
    messagetype   = "";
    content_type  = ""; 
    std::string value = "" ;
/*
 *
 */
    if ( mimetype == "text/plain"){
        value = "5" ; 
        messagetype  = "TextMessage" ; 
        content_type = mimetype ;
        return value ;
    }
    if ( mimetype == "application/octet-stream"){
        value = "3" ; 
        messagetype  = "BytesMessage" ; 
        content_type = mimetype ;
        return value ;
    }
/*
 * First check if there is a map with AMQP annotations , as is the case when Connection Option nest_annotations=true
 * See amqpConnect comment above.
 *
 */
    qpid::types::Variant::Map man = message.getProperties();
    qpid::types::Variant::List mylist ;
    qpid::types::Variant::Map::iterator itm = man.find( "x-amqp-message-annotations" ); 
    std::string strf1 ;
    std::string strf2 ;
    if ( itm != man.end() ){
        strf1   = itm->first ;
        strf2 = itm->second.asString() ;
/*
 * found contains the position of the match or std::string::npos (not-found)
 *
 * strf1 : "x-amqp-message-annotations" 
 * strf2 : "{x-opt-jms-dest:0, x-opt-jms-msg-type:5}"
 *
 */
        std::size_t found = strf2.find( "x-opt-jms-msg-type" ); 
        if ( found != std::string::npos ){
            found = strf2.find( ":", found );
            if ( found != std::string::npos ){
                value = strf2.substr ( found + 1 , 1 );
                #ifdef DEBUG
                    std::cout <<  "checkAMQP2JMSmessageType - found x-opt-jms-msg-type value in x-amqp-message-annotations :  " << value << std::endl ;
                #endif
            }
        }
    }else{
        qpid::types::Variant::Map mp = message.getProperties();
        qpid::types::Variant::Map::iterator it = mp.find( "x-opt-jms-msg-type" );
        if ( it != mp.end() ){
            value = it->second.asString() ;
            #ifdef DEBUG
            std::cout <<  "checkAMQP2JMSmessageType - found x-opt-jms-msg-type value in general message.getProperties() :  " << value << std::endl ;
            #endif
        }else{
            it = mp.find( "x_HYPHEN_opt_HYPHEN_jms_HYPHEN_msg_HYPHEN_type" );
            if ( it != mp.end() ){
                value = it->second.asString() ;
                #ifdef DEBUG
                    std::cout <<  "checkAMQP2JMSmessageType - found x_HYPHEN_opt_HYPHEN_jms_HYPHEN_msg_HYPHEN_type value in general message.getProperties() :  " << value << std::endl ;
                #endif
            }
        } 
    }
     
/*
    TextMessage 
        A TextMessage will be sent using an amqp-value body section containing a
        utf8 encoded string of the body text, or null if no body text is set. The message
        annotation with symbol key of “x-opt-jms-msg-type” will be set to a byte value
        of 5.
    BytesMessage 
         A BytesMessage will be sent using a data body section containing the raw
        bytes from the BytesMessage body, with the properties section contenttype
        field set to the symbol value “application/octet-stream”. The message
        annotation with symbol key of “x-opt-jms-msg-type” will be set to a byte value
        of 3.
    MapMessage 
        A MapMessage body will be sent using an amqp-value body section
        containing a single map value. Any byte[] values in the MapMessage body will
        be encoded as binary entries in the map. The message annotation with
        symbol key of “x-opt-jms-msg-type” will be set to a byte value of 2.
    StreamMessage 
         A StreamMessage will be sent using an amqp-sequence body section
        containing the entries in the StreamMessage body. Any byte[] entries in the
        StreamMessage body will be encoded as binary entries in the sequence. The
        message annotation with symbol key of “x-opt-jms-msg-type” will be set to a
        byte value of 4.
    ObjectMessage 
         An ObjectMessage will be sent using an data body section, containing the
        bytes from serializing the ObjectMessage body using an ObjectOutputStream,
        with the properties section contenttype field set to the symbol value
        “application/x-java-serialized-object”. The message annotation with symbol
        key of “x-opt-jms-msg-type” will be set to a byte value of 1.

    Message 
         A plain JMS Message has no body, and will be sent as an amqp-value body
        section containing a null. The message annotation with symbol key of “x-optjms-
        msg-type” will be set to a byte value of 0.

*/
    if ( value == "5" ){
        messagetype = "TextMessage" ;
        return value ;
    }
    if ( value == "3" ){ 
        messagetype  = "BytesMessage" ; 
        content_type = "application/octet-stream"  ;
        return value ;
    }
    if ( value == "2" ){
        messagetype = "MapMessage" ;
        return value ;
    }
    if ( value == "4" ){
        messagetype = "StreamMessage" ;
        return value ;
    }
    if ( value == "1" ){ 
        messagetype  = "ObjectMessage" ;
        content_type = "application/x-java-serialized-object" ;
        return value ;
    }
    if ( value == "0" ){ 
        messagetype = "Message" ;
        return value ;
    }
    if ( mimetype == "application/octet-stream"){
        value = 3 ; 
        messagetype  = "BytesMessage" ; 
        content_type = mimetype ;
        return value ;
    }
    value = "N/F";
#ifdef DEBUG 
    std::cout <<  "checkAMQP2JMSmessageType : x-opt-jms-msg-type -> MessageType " << value << " -> " << messagetype << "   mimetype ->  " << content_type << std::endl ;
#endif
    return value ;
 }
// 
int16_t amqpBye () {
    connection.close();
#ifdef DEBUG
    std::cout <<  "amqpBye " << std::endl ;
#endif
    return 0;
}
//
void bulkMessage ( Message &message ) {
//
    std::string msg  ( MAX_MESSAGE_LENGTH, '\0' );
    msg = message.getContentBytes () ;
    int16_t length = message.getContentSize() ; 
    int16_t lenmsg = msg.length() ; 
    std::string messageUUID ;
    const char *p = message.getContentPtr() ;
//
    int k = 15 ;
    std::cout <<  " k is 15 -> " << k << std::endl; 
    qpid::types::Variant::Map mp = message.getProperties();
    std::string properties = "";
    for ( qpid::types::Variant::Map::iterator it=mp.begin(); it !=mp.end() ; ++it ){
          std::string name  = it->first ;
          std::string value = it->second ;
          if ( name == APL_MESSAGE_UUID ){
              messageUUID = value ;
          }
    }
    std::cout <<  "Start of bulkMessage ======================================== "<< std::endl ;
    std::cout <<  "  message.getContentBytes ()    "<< msg    << std::endl ;
    std::cout <<  "  message.getContentSize  ()    "<< length << std::endl ;
    std::cout <<  "  msg                           "<< msg    << std::endl ;
    std::cout <<  "  msg.length()                  "<< lenmsg << std::endl ; 
    for (int k=0; k<length; k++){
        std::cout <<  " k -> " << k << " -> " << *(p+k) << " 0x"<< hex( *(p+k) ) << std::endl ; 
    }
    std::cout <<  "End   of bulkMessage ======================================== "<< std::endl ;
}
//
void setProperty( Message& message, const std::string& property )
{
        std::string name;
        std::string value;
        if ( nameval(property, name, value) ) {
            message.getProperties()[name] = value;
            message.getProperties()[name].setEncoding("utf8");
#ifdef DEBUG             
            std::cout <<  "setProperty is : " << property << std::endl ;
#endif
        }else {
            std::cout <<  "WARNING setProperty property not set : " << property  << std::endl ;
        }
}
//
void setProperties( const string_vector &properties, Message& message ) 
{
        for (string_vector::const_iterator i = properties.begin(); i != properties.end(); ++i) {
            setProperty(message, *i);
        }
}
//
void setEntries( const string_vector &properties, qpid::types::Variant::Map &map ) 
{
        for (string_vector::const_iterator i = properties.begin(); i != properties.end(); ++i) {
            std::string name;
            std::string value;
            if (nameval(*i, name, value)) {
                map[name] = value;
            }
        }
}
//
bool nameval(const std::string& in, std::string& name, std::string& value)
{
        std::string::size_type i = in.find("=");
        if (i == std::string::npos) {
            name = in;
            return false;
        } else {
            name = in.substr(0, i);
            if (i+1 < in.size()) {
                value = in.substr(i+1);
                return true;
            } else {
                return false;
            }
        }
}
//


