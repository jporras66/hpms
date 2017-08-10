#include <iostream>
#include "xmlcfg.h"
#include "common.h"
#include "message.h"

int main(int argc, char *argv[]) {
    try
    {
        //
        char *pPath;
        pPath = getenv ("RESOURCES");
        if (pPath == NULL){
            printf( "Please execute ini<apl>.sh to load environment variables :-) !!\n" );
            return EXIT_FAILURE ;
        }
        std::string path( pPath );
        std::string  ifilenameBrk = path + "broker_connection.xml";
        std::string  ifilenameQue = path + "request.xml";
        std::string  ifilenameDb  = path + "db_connection.xml";
    /*
             C Style
    */
        std::cout << "======================= " << std::endl ;
        std::cout << "Testing - C Style Calls " << std::endl ;
        std::cout << "======================= " << std::endl ;
        std::cout << "Function configBrokerConnectionCC for : " + ifilenameBrk << std::endl ;
    //
        char vhost [MAX_HOSTNAME_LENGTH];
        char vport [MAX_HOSTNAME_LENGTH];
        char errstatus [MAX_ERRSTATUS_LENGTH];
        memset( vhost ,'\0', MAX_HOSTNAME_LENGTH );
        memset( vport ,'\0', MAX_PORT_LENGTH );
        memset( errstatus ,'\0', MAX_ERRSTATUS_LENGTH ); 
    //
        int rc = configBrokerConnectionCC ( ifilenameBrk.c_str() , vhost, vport, errstatus );
        std::string errstatusN( errstatus) ;
        if ( rc == -1 ){
            std::cout << "Error configBrokerConnectionCC rc is : " << rc  << " -> "  + errstatusN  << std::endl ;
            return rc ;
        }
        std::string ohost ( vhost ); 
        std::string oport ( vport ); 
        std::cout << "Success host port : " + ohost + " "+ oport +" "+ errstatusN  << std::endl ;
    //
        std::cout << "======================= " << std::endl ;
    //
        std::cout << "Function configQueueCC for : " + ifilenameQue << std::endl ;
        char q_prg [ MAX_PMG_LENGTH ];
        char q_i [ MAX_QUEUENAME_LENGTH ];
        char q_o [ MAX_QUEUENAME_LENGTH ];
        char q_e [ MAX_QUEUENAME_LENGTH ];
        char q_t [ MAX_QUEUENAME_LENGTH ];
        rc = configQueueCC ( ifilenameQue.c_str() , q_prg, q_i, q_o, q_e, q_t, errstatus );
        string er2 ( errstatus );
        if ( rc == -1 ){
            std::cout << "Error congifQueueCC rc is : " << rc  << " -> "  + er2 << std::endl ;
            return rc ;
        }
        string er1 ( errstatus );
        string qprg ( q_prg );
        string qi ( q_i );
        string qo ( q_o );
        string qe ( q_e );
        string qt ( q_t );
        std::cout << "Success program queue_i queue_o queue_e queue_t : " + qprg + " " + qi + " "+ qo +" "+ qe+" "+qt +" " + er1  << std::endl ;
    /*
             C++ Style
    */
        std::cout << "========================= " << std::endl ;
        std::cout << "Testing - C++ Style Calls " << std::endl ;
        std::cout << "========================= " << std::endl ;
        std::cout << "Function configBrokerConnection for : " + ifilenameBrk << std::endl ;
        XmlCfgBRK cfg ;
        rc = configBrokerConnection ( ifilenameBrk , cfg , errstatusN );
        if ( rc == -1 ){
            std::cout << "Error configBrokerConnection rc is : " << rc  << " -> "  + errstatusN  << std::endl ;
            return rc ;
        }
        std::cout << "Success host port : " + cfg.bc_host + " "+ cfg.bc_port +" "+ errstatusN  << std::endl ;
    //
        std::cout << "======================= " << std::endl ;
    //
        std::cout << "Function configQueue for : " + ifilenameQue << std::endl ;
        XmlCfgQUE cfq ;
        rc = configQueue ( ifilenameQue, cfq, errstatusN );
        if ( rc == -1 ){
            std::cout << "Error congifQueue rc is : " << rc  << " -> "  + errstatusN << std::endl ;
            return -1 ;
        }
        std::cout << "Success q_prgname  : " + cfq.q_prgname  << std::endl ;
        std::cout << "        q_input    : " + cfq.q_input    << std::endl ;
        std::cout << "        q_output   : " + cfq.q_output   << std::endl ;
        std::cout << "        q_error    : " + cfq.q_error    << std::endl ;
        std::cout << "        q_timeout  : " + cfq.q_timeout  << std::endl ;
    //
        std::cout << "======================= " << std::endl ;
    //
        std::cout << "Function configDbConnection for : " + ifilenameDb << std::endl ;
        XmlCfgDB cfdb ;
        rc = configDbConnection ( ifilenameDb, cfdb, errstatusN ); 
        if ( rc == -1 ){
            std::cout << "Error configDbConnection rc is : " << rc  << " -> "  + errstatusN << std::endl ;
            return -1 ;
        }
        std::cout << "Success db_provider  : " + cfdb.db_provider   << std::endl ;
        std::cout << "        db_name      : " + cfdb.db_name       << std::endl ;
        std::cout << "        db_user      : " + cfdb.db_user       << std::endl ;
        std::cout << "        db_password  : " + cfdb.db_password   << std::endl ;

    }
//
    catch (std::exception &e)
    {
        std::cout << "Error: " << e.what() << "\n";
    }
    return 0;
}
