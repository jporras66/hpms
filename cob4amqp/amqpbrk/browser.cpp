#include <cstring>
#include <stdlib.h>
#include "amqpbrk.hpp"
#include "xmlcfg.h"

int main(int argc, char *argv[])
{
    if ( argc != 4 )
    {
        std::cerr << "usage is : " << argv[0] << " <broker_connection.xml> <queue_name> <number_of_msgs>" << std::endl;
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
    std::string brokerName( argv[1] );
    std::string queueName( argv[2] );
    std::string str_msg_number ( argv[3] );
    int MAX_MSG_NUMBER =  atoi ( str_msg_number.c_str() ) ;
//
    XmlCfgBRK bcc;
    std::string  ifilenameBrk ( MAX_FILENAME_LENGTH, '\0' ) ; 
    ifilenameBrk= path + brokerName ;
    std::string errstatus ( MAX_ERRSTATUS_LENGTH, '\0' ); 
    std::string response  ( MAX_MESSAGE_LENGTH, '\0' );
//
    int rc = configBrokerConnection    ( ifilenameBrk , bcc , errstatus );
    if ( rc == -1 ){
        std::cout << "Error configBrokerConnection rc is : " << rc  << " -> "  + errstatus << std::endl ;
        return rc ;
    }
//
    rc = amqpConnect ( bcc.bc_host, bcc.bc_port, DEFAULT_AMQP_VERSION , errstatus );
    if (  rc == -1) {
        std::cerr << "browser.amqpConnect ->Error Connecting to url :" << bcc.bc_host << ":" << bcc.bc_port << ":" << DEFAULT_AMQP_VERSION << std::endl ;
        return EXIT_FAILURE ;
    }
//
    int msg_number = 0;
    while ( true ){
        Message  message ; 
        rc = amqpBrowseMessage ( queueName , message, errstatus ) ;
        response = message.getContent() ;
        std::cout <<  "amqpBrowseMessage->receiving from : " + queueName << " | mime is : " + message.getContentType() << " | message is : " + message.getContent() << " length : " + response.length() << std::endl ;
//
        qpid::types::Variant::Map mp = message.getProperties();
        for ( qpid::types::Variant::Map::iterator it=mp.begin(); it !=mp.end() ; ++it ){
            std::string name  = it->first ;
            std::string value = it->second ;  
            std::cout <<  "Property is : name / value  : " << name  << "  ->  " << value << std::endl ;
        }
        msg_number++ ;
        if  ( msg_number >= MAX_MSG_NUMBER ) break ; 
//
    }
//
    rc = amqpBye();
    std::cout <<  "before return " << std::endl ;
    return EXIT_SUCCESS ;
}
