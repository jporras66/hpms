/*
 * Arguments :
 * 
 *             xml config files are located from $RESOURCES directory (otherwise --> error) .
 *
 * <broker_connection.xml>    	To load broker connection properties
 *
 * <queue_name.xml>       		To load program name and queues descriptions
 *
 *                          	f.e. : 	request_1.xml has defined the following cobol program 
 *                          			<prgname>req_trx</prgname>  --> req_trx.so
 *                          	Then :
 *                               		queueListen (queueListen.c) --> driver.o (driver.cob) --> req_trx.so (req_trx.cob)
 *                                      Loads request_1.xml and pass control to req_trx.so (req_trx.cob)
 */
#include <cstring>
#include "amqpbrk.hpp"
#include "xmlcfg.h"
#include "cob4brk.h"
//
int main(int argc, char *argv[])
{
    if ( argc != 4 )
    {
        std::cout << "usage is : " << argv[0] << " <db_connection.xml> <broker_connection.xml> <trx_name.xml>" << std::endl;
        return EXIT_FAILURE ;
    }
//
    char *pPath;
    pPath = getenv ("RESOURCES"); 
    if (pPath == NULL){
        std::cout << "RESOURCE env variable does not exist !!" << std::endl ;
        std::cout << "Please execute ini<apl>.sh to load environment variables :-) !!" << std::endl ;
        return EXIT_FAILURE ;
    }
    std::string path( pPath );
    std::string dbNameFile     ( argv[1] );
    std::string brokerNamefile ( argv[2] );
    std::string queueNameFile  ( argv[3] );
//
    std::string ifilenameDb  ( MAX_FILENAME_LENGTH,  '\0' ) ;
    std::string ifilenameBrk ( MAX_FILENAME_LENGTH,  '\0' ) ; 
    std::string ifilenameQue ( MAX_FILENAME_LENGTH,  '\0' ) ; 
    std::string errstatus    ( MAX_ERRSTATUS_LENGTH, '\0' ) ; 
//
    ifilenameDb = path + dbNameFile ;
    ifilenameBrk= path + brokerNamefile ;
    ifilenameQue= path + queueNameFile ;
    
    if ( ! file_exist ( ifilenameDb.c_str() ) ){
        std::cout << " Database Config file " << ifilenameDb << " does not exist   !!" << std::endl ;       
        std::cout << " Please verify ini<apl>.sh to load environment variables :-) !!" << std::endl ;
        return EXIT_FAILURE ;
    }
    if ( ! file_exist ( ifilenameBrk.c_str() ) ){
        std::cout << " Broker Config file " << ifilenameBrk << " does not exist  !!" << std::endl ;       
        std::cout << " Please verify ini<apl>.sh to load environment variables :-) !!" << std::endl ;
        return EXIT_FAILURE ;
    }
    if ( ! file_exist ( ifilenameQue.c_str() ) ){
        std::cout << " Queue  Config file " << ifilenameQue << " does not exist  !!" << std::endl ;       
        std::cout << " Please verify ini<apl>.sh to load environment variables :-) !!" << std::endl ;
        return EXIT_FAILURE ;
    }
// 
    XmlCfgDB cfdb ;
    int16_t rc = configDbConnection ( ifilenameDb, cfdb, errstatus ); 
    if ( rc == -1 ){
        std::cout << "Error configDbConnection rc is : " << rc  << " -> "  + errstatus << std::endl ;
        return EXIT_FAILURE ;
    }   
    XmlCfgBRK cfbrk ;
    rc = configBrokerConnection ( ifilenameBrk , cfbrk , errstatus );
    if ( rc == EXIT_FAILURE ){
        std::cout << "Error configBrokerConnection rc is : " << rc  << " -> "  + errstatus  << std::endl ;
        return EXIT_FAILURE ;
    }
    XmlCfgQUE cfque ;
    rc = configQueue ( ifilenameQue, cfque, errstatus );
    if ( rc == -1 ){
        std::cout << "Error congifQueue rc is : " << rc  << " -> "  + errstatus << std::endl ;
        return EXIT_FAILURE ;
    }
/*
 *  
 */ 
    rc = amqpConnect ( cfbrk.bc_host, cfbrk.bc_port, DEFAULT_AMQP_VERSION , errstatus );
    if (  rc == -1) {
        std::cout << "amqpConnect ->Error Connecting to url :" << cfbrk.bc_host << ":" << cfbrk.bc_port << ":" << DEFAULT_AMQP_VERSION << std::endl ;
        return EXIT_FAILURE ;
    }
//
    cob_init(0, NULL);
//
    char errstatusCC [ MAX_ERRSTATUS_LENGTH ] ;
    memset ( errstatusCC, '\0', MAX_ERRSTATUS_LENGTH );
    rc = driver (  cfdb.db_connstr.c_str(), cfdb.db_user.c_str(), cfdb.db_password.c_str() , queueNameFile.c_str() ,  errstatusCC );
    errstatus = errstatusCC ; 
    std::cout << "driver rc is : " << rc  << " errstatus : " << errstatus << std::endl ;
//
    rc = amqpBye ( );
    if ( rc == -1) {
        std::cout << "amqpBye ->Error :" <<  cfbrk.bc_host << ":" << cfbrk.bc_port << ":" << DEFAULT_AMQP_VERSION << std::endl ;
         return EXIT_FAILURE ;
    }
//
    rc = amqpBye();
    return EXIT_FAILURE ;
    
}
