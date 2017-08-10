#include <cstring>
#include <libpq-fe.h>
#include <thread>
#include <mutex> 
#include "amqpbrk.hpp"
#include "xmlcfg.h"
#include "sleep.h"
//
struct Counter {
    std::mutex mutex;
    int value;
    int MAX_VALUE ;

    Counter( int val )  {
        MAX_VALUE = val ;
        value = 0 ; 
    }

    bool increment(){
        mutex.lock();
            int k = value + 1 ;
            if ( k > MAX_VALUE ) return false ;
            ++value;
        mutex.unlock();
        return true ;
    }
    void decrement(){
        mutex.lock();
            --value;
        mutex.unlock();
    }
};
//
Counter counter ( MAX_NUM_THREADS ) ;
//
int timeout ( const XmlCfgDB &cfdb, const std::string messageUUID, const Message  &message , const std::string q_output , const int16_t timeout_seconds  );
//
int main(int argc, char *argv[]) {
    if ( argc != 5 ) {
        std::cerr << "usage is : " << argv[0] << " <dbpool_connection.xml> <broker_connection.xml> <timeout_queue.xml> <timeout_time> in seconds" << std::endl;
        return EXIT_FAILURE ;
    }
//    
    std::cout << "Starting  ... " << argv[0] << std::endl ;
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
    std::string q_inputFile  ( argv[3] );
    int16_t timeout_seconds = atoi ( argv[4] ) ;
//
    std::string ifilenameDb  = string ( path + dbNameFile ) ;
    std::string ifilenameBrk = string ( path + brokerNamefile ) ; 
    std::string ifilenameQue ( path + q_inputFile ) ; 
    std::string errstatus    ( MAX_ERRSTATUS_LENGTH, '\0' ) ; 
//
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
        return -1 ;
    }   
    XmlCfgBRK cfbrk ;
    rc = configBrokerConnection ( ifilenameBrk , cfbrk , errstatus );
    if ( rc == -1 ){
        std::cout << "Error configBrokerConnection rc is : " << rc  << " -> "  + errstatus  << std::endl ;
        return -1 ;
    }
    XmlCfgQUE cfque ;
    rc = configQueue ( ifilenameQue, cfque, errstatus );
    if ( rc == -1 ){
        std::cout << "Error congifQueue rc is : " << rc  << " -> "  + errstatus << std::endl ;
        return -1 ;
    }
/*
 * Broker Connect 
 */ 
    rc = amqpConnect ( cfbrk.bc_host, cfbrk.bc_port, DEFAULT_AMQP_VERSION , errstatus );
    if (  rc == -1 ) {
        std::cerr << "amqpConnect ->Error Connecting to url :" << cfbrk.bc_host << ":" << cfbrk.bc_port << ":" << DEFAULT_AMQP_VERSION << std::endl ;
        return EXIT_FAILURE ;
    }
//
    std::string q_input  = cfque.q_input  ;
    std::string q_output = cfque.q_output ;
//
    while ( true ){
        Message  message ;
        rc = amqpRecvMessage ( q_input , message, errstatus ) ;
        std::string msg = message.getContent() ;
        std::string mimetype = message.getContentType();
        std::string messageUUID ; 
#ifdef DEBUG
        std::cout <<  "receiving from queue : " + q_input + " -> " + msg << " length-> " << rc
                  << " ->mime : " + mimetype  << std::endl ;
#endif
//
        qpid::types::Variant::Map mp = message.getProperties();
        for ( qpid::types::Variant::Map::iterator it=mp.begin(); it !=mp.end() ; ++it ){
            std::string name  = it->first ;
            std::string value = it->second ; 
            if ( name == APL_MESSAGE_UUID){ 
                messageUUID = string ( value.c_str() ) ;
            }
#ifdef DEBUG
            std::cout <<  "    Property is : name/value : " << name  << " - " << value << std::endl ;
#endif
        }
//
        while( true ){
            if ( counter.increment() ){
                std::thread t( timeout, cfdb, messageUUID, message, q_output, timeout_seconds  );
                t.detach();
                //int rc = timeout ( cfdb, messageUUID, message, q_output, timeout_seconds  );
                string dummy ; 
                cin >> dummy ;
                break;
            }else{
                /*
                 *      micro_sleep ( long usec ) or to_sleep ( int sec )
                 */
                micro_sleep ( 1000000L ) ;
            }
        }
    }
    rc = amqpBye();
    std::cout <<  "before return " << std::endl ;
//
    return EXIT_SUCCESS ;
}

int timeout ( const XmlCfgDB &cfdb, const std::string messageUUID, const Message  &message , const std::string q_output , const int16_t timeout_seconds ) {   
//
    std::cout << std::endl;
    std::cout << "timerThread Starts id : " << std::this_thread::get_id() << " - sleeps for " << timeout_seconds <<  " seconds ..." <<std::endl ;
#ifdef DEBUG
    std::cout << "timerThread parameters " << std::endl ;
    std::cout << "     queueOutput   " << q_output << std::endl ;
    std::cout << "     messageUUID   " << messageUUID << std::endl ;
#endif 
/*
 * Execute the timeout
 */
    micro_sleep ( (1000000L)*timeout_seconds ) ;
/*
 * Note : Postgresql :  A Connection == session. 
 * So it's better to implement connection pooling for every thread PQconnectdb (  PGBOUNCER for example )
 *
 */ 
    std::string connString = "dbname=" + cfdb.db_name + " host="+ cfdb.db_host + " port="+ cfdb.db_port +" user=" + cfdb.db_user + " password=" + cfdb.db_password ; 
    PGconn *dbconn = PQconnectdb( connString.c_str() );
    if ( PQstatus( dbconn ) == CONNECTION_BAD) {
        std::cout << "PostgreSql.Connection   -> Not connected to database " << connString << std::endl ;
        return EXIT_FAILURE ;
    }
/*
 * Test if pqsql is threadsafe : 1 means thread-safe, 0 means not thread-safe . response : 1 It is thread-safe. 
 *
 * int rc = PQisthreadsafe() ;
 * std::cout <<  "PQserverVersion  : " << PQserverVersion(dbconn) << " PQlibVersion() : " << PQlibVersion() << std::endl ;
 * std::cout <<  "PQisthreadsafe() : " << rc << " - " << ( rc ? "Thread-safe" : "NOT Thread-safe" ) << std::endl ;
 *
 */
    PGresult *res = PQexec(dbconn, "BEGIN");      
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::cout << "BEGIN command failed !!! "  << std::endl ;      
        PQclear(res);
        return EXIT_FAILURE ;
    }  
//
   const char *paramValues[1];
   std::string stm = "SELECT uuid, timestamp FROM timeout where uuid = $1::varchar";
   char timestamp_c [ 30 ] ;
   memset ( timestamp_c, '\0', 30 );
   char messageUUID_c [ MAX_MESSAGE_ID_LENGTH ] ; 
   strcpy ( messageUUID_c, messageUUID.c_str()  ) ;
   paramValues[0]  = messageUUID_c  ;
//
   res = PQexecParams( dbconn, stm.c_str() , 1 , NULL, paramValues, NULL, NULL, 0 );
//#ifdef DEBUG
   std::cout <<  "timeout.uuid      " << PQgetvalue(res, 0, 0) << std::endl ;
   std::cout <<  "timeout.timestamp " << PQgetvalue(res, 0, 1) << std::endl ;
//#endif 
/* 
 * Timeout exist
 */
   if ( PQresultStatus(res) == PGRES_TUPLES_OK ) {    
        PQclear(res);
/*
 * delete the timeout 
 */
        std::string stm = "DELETE from timeout where uuid = $1::varchar";
        paramValues[0]  = messageUUID_c  ;
        res = PQexecParams( dbconn, stm.c_str() , 1 , NULL, paramValues, NULL, NULL, 0 );
        PQclear(res); 
/*
 * Create broker session and sender
 */
        std::string dummy = q_output + " ; {create: always} ";
        Session session = amqpGetSession () ;
        Sender  sender  = session.createSender( dummy );
/*
 * Create message before send it 
 */
        std::string timestamp = string ( timestamp_c ) ;
        std::string msgstr = "!! message timeout !!" + timestamp + " - " + message.getContent();
        Message message2send = Message ( message ) ;
        message2send.setContent( msgstr ) ;
        sender.send( message2send );
        session.acknowledge( message2send );
        session.close() ;
        sender.close() ;
   } 
//
    res = PQexec(dbconn, "COMMIT");      
    if ( PQresultStatus(res) != PGRES_COMMAND_OK ) {
        std::cout << "COMMIT command failed !!! "  << std::endl ;        
        PQclear(res);
        return EXIT_FAILURE ;
    }  
//
    PQclear(res); 
    PQfinish(dbconn);
//
    counter.decrement();
    std::cout << "timerThread Ends   id : " << std::this_thread::get_id() << std::endl;
//
    return EXIT_SUCCESS ;
}

