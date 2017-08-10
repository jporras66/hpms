#include <cstring>
#include <libpq-fe.h>
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
    std::string ifilenameDb  ( MAX_PATH_LENGTH,  '\0' ) ;
    std::string ifilenameBrk ( MAX_PATH_LENGTH,  '\0' ) ; 
    std::string ifilenameQue ( MAX_PATH_LENGTH,  '\0' ) ; 
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
    PGconn *dbconn;
    std::string connString = "dbname=" + cfdb.db_name + " host="+ cfdb.db_host + " port="+ cfdb.db_port +" user=" + cfdb.db_user + " password=" + cfdb.db_password ; 
    dbconn = PQconnectdb( connString.c_str() );
    if ( PQstatus( dbconn ) == CONNECTION_BAD) {
        std::cout << "PostgreSql.Connection -> Unable to connect to database " << connString << std::endl ;
        return EXIT_FAILURE ;
    }else{
        std::cout << "PostgreSql.Connection -> Succsses  " << connString << std::endl ;
    }
//
    cob_init(0, NULL);
//
    char errstatusCC [ MAX_ERRSTATUS_LENGTH ] ;
    memset ( errstatusCC, '\0', MAX_ERRSTATUS_LENGTH );
    rc = driver (  queueNameFile.c_str() ,  errstatusCC );
    errstatus = errstatusCC ; 
    std::cout << "driver rc is : " << rc  << " errstatus : " << errstatus << std::endl ;
//
    rc = amqpBye ( );
    if ( rc == -1) {
        std::cout << "amqpBye ->Error :" <<  cfbrk.bc_host << ":" << cfbrk.bc_port << ":" << DEFAULT_AMQP_VERSION << std::endl ;
         return EXIT_FAILURE ;
    }
//
    PQfinish(dbconn);
    rc = amqpBye();
    return EXIT_FAILURE ;
    
}
