#include <cstring>
//#include <boost/uuid/uuid.hpp>
#include "amqpbrk.hpp"
#include "xmlcfg.h"

using namespace qpid::messaging;
using namespace qpid::types;
//using namespace boost::uuids;

int main(int argc, char** argv) {

    if ( argc != 6 )
    {
        std::cerr << "usage is : " << argv[0] << " <broker_connection.xml> <queue_name> <clientIp> <clientPort> <num_messages>" << std::endl;
        return EXIT_FAILURE ;
    }
//
    char *pPath;
    pPath = getenv ("RESOURCES"); 
    if (pPath == NULL){
        std::cout << "Please execute ini<apl>.sh to load environment variables :-) !!" << std::endl ;
        return EXIT_FAILURE ;
    }
    std::string path( pPath );
    std::string brokerName ( argv[1] );
    std::string queueName  ( argv[2] );
    std::string clientIp   ( argv[3] );
    std::string clientPort ( argv[4] );
    std::string dummy      ( argv[5] );
    int  numMessages = atoi( dummy.c_str() );
//
    XmlCfgBRK bcc;
    std::string  ifilenameBrk ( MAX_FILENAME_LENGTH, '\0' ) ; 
    ifilenameBrk= path + brokerName ;
    std::string errstatus ( MAX_ERRSTATUS_LENGTH, '\0' );
//
    int rc = configBrokerConnection    ( ifilenameBrk , bcc, errstatus );
    if ( rc == -1 ){
        std::cout << "Error configBrokerConnection rc is : " << rc  << " -> "  + errstatus << std::endl ;
        return rc ;
    }
//
    rc = amqpConnect ( bcc.bc_host, bcc.bc_port, DEFAULT_AMQP_VERSION , errstatus );
    if (  rc == -1) {
        std::cerr << "producer.amqpConnect ->Error Connecting to url :" << bcc.bc_host << ":" << bcc.bc_port << ":" << DEFAULT_AMQP_VERSION << std::endl ;
        return EXIT_FAILURE ;
    }
//
    for (int i=0; i<numMessages;i++){
        ostringstream ss;
        ss << i ; 
        std::string smsg = "HELLO world " + ss.str() + " !!";
        std::string mimetype = "text/plain";
/*
 * these properties : text-plain-message-id and myproperty-id are a custom properties
 */
        Message message ;
        string_vector properties;
        string messageUUID = Uuid(true).str();
        std::string property ;
//
        property="clientIp="+clientIp ;
        properties.push_back ( property ) ;
        property="clientPort="+clientPort ;
        properties.push_back ( property ) ;
        property="messageUUID=" + messageUUID ;
        properties.push_back ( property ) ;
//
        message.setContentType  ( mimetype );
        message.setContent ( smsg ) ;
        message.setMessageId( messageUUID ) ;
//
        rc = amqpSendMessage     ( queueName, message , properties , errstatus );
        std::cout <<  "producer.amqpSendMessage - "<< queueName << " : " << smsg  << " : " << mimetype << std::endl ;
    }
//
    for (int i=0; i<numMessages;i++){
        ostringstream ss;
        ss << i ; 
        std::string smsg = "BYE world " + ss.str(); + " !!";
        string mimetype ="application/octet-stream";
/*
 * these properties : application-octet-stream-message-id is a custom property
 */
        Message message ;
        string_vector properties;
        string messageUUID = Uuid(true).str();
        std::string property ;
//
        property="clientIp="+clientIp ;
        properties.push_back ( property ) ;
        property="clientPort="+clientPort ;
        properties.push_back ( property ) ;
        property="messageUUID=" + messageUUID ;
        properties.push_back ( property ) ;
//
        message.setContentType  ( mimetype );
        message.setContent ( smsg ) ;
        message.setMessageId( messageUUID ) ;
//
        rc = amqpSendMessage     ( queueName, message , properties , errstatus );
        std::cout <<  "producer.amqpSendMessage - "<< queueName << " : " << smsg  << " : " << mimetype << std::endl ;
    }
//
    rc = amqpBye();
    std:cout << "Before Return rc is : " << rc << std::endl ;
    return EXIT_SUCCESS ;
}

