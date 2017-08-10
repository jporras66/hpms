#include "xmlcfg.h"

/*
 * Source from http://www.boost.org/doc/libs/1_42_0/libs/property_tree/examples/debug_settings.cpp
 */
 
/*
 *   C++-Style Calls
 */ 

uint16_t configBrokerConnection ( const std::string &filename, XmlCfgBRK &cfg, std::string &errstatus ) {
    
    errstatus = "" ;
    cfg.loadConfigBrokerConnection( filename );
    cfg.saveConfigBrokerConnection( filename + ".c_out");

    if ( cfg.bc_host.length() > MAX_HOSTNAME_LENGTH ) {
        std::stringstream ss ;
        ss << "bc_host up to " << MAX_HOSTNAME_LENGTH-1 << " characters !! :-) !!" ;
        errstatus = ss.str();
        return -1 ;
    }

    if ( cfg.bc_port.length() > MAX_PORT_LENGTH ) {
        std::stringstream ss ;
        ss << "bc_port up to " << MAX_PORT_LENGTH-1 << " characters !! :-) !!" ;
        errstatus = ss.str();
        return -1 ;
    }
}
//


uint16_t configQueue ( const std::string &filename, XmlCfgQUE &cfg , std::string &errstatus ){
//
    cfg.loadConfigQueue ( filename );
    cfg.saveConfigQueue ( filename + ".c_out");
//
    if ( cfg.q_prgname.length() > MAX_PMG_LENGTH ) {
        std::stringstream ss ;
        ss << "q_prgname up to " << MAX_PMG_LENGTH - 1 << " characters !! :-) !!" ;
        errstatus = ss.str();
        return -1 ;
    }
    if ( cfg.q_input.length() > MAX_QUEUENAME_LENGTH) {
        std::stringstream ss ;
        ss << "q_input queue name up to " << MAX_QUEUENAME_LENGTH-1 << " characters !! :-) !!" ;
        errstatus = ss.str();
        return -1 ;
    }
    if ( cfg.q_output.length() > MAX_QUEUENAME_LENGTH) {
        std::stringstream ss ;
        ss << "q_output up to " << MAX_QUEUENAME_LENGTH-1 << " characters !! :-) !!" ;
        errstatus = ss.str();
        return -1 ;
    }
    if ( cfg.q_error.length() > MAX_QUEUENAME_LENGTH) {
        std::stringstream ss ;
        ss << "q_error up to " << MAX_QUEUENAME_LENGTH-1 << " characters !! :-) !!" ;
        errstatus = ss.str();
        return -1 ;
    }
    if ( cfg.q_timeout.length() > MAX_QUEUENAME_LENGTH) {
        std::stringstream ss ;
        ss << "q_timeout up to " << MAX_QUEUENAME_LENGTH-1 << " characters !! :-) !!" ;
        errstatus = ss.str();
        return -1 ;
    }
    return 0 ;
}

uint16_t configDbConnection    ( const std::string &filename, XmlCfgDB  &cfg, std::string &errstatus) {
//
    cfg.loadConfigDb ( filename );
    cfg.saveConfigDb ( filename + ".c_out");
    if ( cfg.db_provider.length() > MAX_DBNAME_LENGTH ) {
        std::stringstream ss ;
        ss << "db_provider MAX_DBNAME_LENGTH up to " << MAX_DBNAME_LENGTH-1 << " characters !! :-) !!" ;
        errstatus = ss.str();
        return -1 ;
    }
    if ( cfg.db_name.length() > MAX_DBNAME_LENGTH) {
        std::stringstream ss ;
        ss << "db_name MAX_DBNAME_LENGTH up to " << MAX_DBNAME_LENGTH-1 << " characters !! :-) !!" ;
        errstatus = ss.str();
        return -1 ;
    }
    if ( cfg.db_user.length() > MAX_DBNAME_LENGTH ) {
        std::stringstream ss ;
        ss << "db_user MAX_DBNAME_LENGTH up to " << MAX_DBNAME_LENGTH-1 << " characters !! :-) !!" ;
        errstatus = ss.str();
        return -1 ;
    }
    if ( cfg.db_password.length() > MAX_DBNAME_LENGTH ) {
        std::stringstream ss ;
        ss << "db_password MAX_DBNAME_LENGTH up to " << MAX_DBNAME_LENGTH-1 << " characters !! :-) !!" ;
        errstatus = ss.str();
        return -1 ;
    } 
    return 0 ;
//
}

//
void XmlCfgBRK::loadConfigBrokerConnection (const std::string &filename)
{
    using boost::property_tree::ptree;
    ptree pt;
    read_xml(filename, pt);
    bc_provider       = pt.get<std::string>("BrokerConnection.provider");
    bc_host           = pt.get<std::string>("BrokerConnection.host");
    bc_transport      = pt.get<std::string>("BrokerConnection.transport");
    bc_port           = pt.get<std::string>("BrokerConnection.port");
    bc_user           = pt.get<std::string>("BrokerConnection.user");
    bc_password       = pt.get<std::string>("BrokerConnection.password");
}

void XmlCfgBRK::saveConfigBrokerConnection(const std::string &filename)
{
    using boost::property_tree::ptree;
    boost::property_tree::xml_writer_settings<char> settings('\t', 1);
    ptree pt;

    pt.put("BrokerConnection.provider",     bc_provider);
    pt.put("BrokerConnection.host",         bc_host);
    pt.put("BrokerConnection.transport",    bc_transport);
    pt.put("BrokerConnection.port",         bc_port);
    pt.put("BrokerConnection.user",         bc_user);
    pt.put("BrokerConnection.password",     bc_password);

    write_xml(filename, pt);

}
//
void XmlCfgQUE::loadConfigQueue (const std::string &filename)
{
    using boost::property_tree::ptree;
    ptree pt;
    read_xml(filename, pt);

    q_prgname    = pt.get<std::string>("Queue.prgname");
    q_input      = pt.get<std::string>("Queue.input");
    q_output     = pt.get<std::string>("Queue.output");
    q_error      = pt.get<std::string>("Queue.error");
    q_timeout    = pt.get<std::string>("Queue.timeout");
}

void XmlCfgQUE::saveConfigQueue(const std::string &filename)
{
    using boost::property_tree::ptree;
    boost::property_tree::xml_writer_settings<char> settings('\t', 1);
    ptree pt;

    pt.put("Queue.prgname", q_prgname);
    pt.put("Queue.input",   q_input);
    pt.put("Queue.output",  q_output);
    pt.put("Queue.error",   q_error);
    pt.put("Queue.timeout", q_timeout);

    write_xml(filename, pt);

}
void XmlCfgDB::saveConfigDb(const std::string &filename)
{
    using boost::property_tree::ptree;
    boost::property_tree::xml_writer_settings<char> settings('\t', 1);
    ptree pt;

    pt.put("DbConnection.provider",     db_provider);
    pt.put("DbConnection.name",         db_name);
    pt.put("DbConnection.user",         db_user);
    pt.put("DbConnection.password",     db_password);
    pt.put("DbConnection.host",         db_host);
    pt.put("DbConnection.port",         db_port);

    write_xml(filename, pt);
}

void XmlCfgDB::loadConfigDb(const std::string &filename)
{
    using boost::property_tree::ptree;
    ptree pt;
    read_xml(filename, pt);
    db_provider      = pt.get<std::string>("DbConnection.provider");
    db_name          = pt.get<std::string>("DbConnection.name");
    db_user          = pt.get<std::string>("DbConnection.user");
    db_password      = pt.get<std::string>("DbConnection.password");
    db_host          = pt.get<std::string>("DbConnection.host");
    db_port          = pt.get<std::string>("DbConnection.port");
    db_connstr       = db_name + "@" + db_host + ":" + db_port ; //HPS@127.0.0.1:5432
}

/*
 *   C-Style Calls
 */ 
uint16_t configBrokerConnectionCC ( const char *ifilename, char *ohost, char *oport, char *errstatus )
{
//
    memset( ohost ,'\0', MAX_HOSTNAME_LENGTH );
    memset( oport ,'\0', MAX_PORT_LENGTH );
    memset( errstatus ,'\0', MAX_ERRSTATUS_LENGTH );
//
    int16_t rc = computeLength ( ifilename, MAX_FILENAME_LENGTH );
    if ( rc == -1) {
        std::stringstream ss ;
        ss << "ifilename up to " << MAX_FILENAME_LENGTH-1 << " characters !! :-) !!" ;
        std::string dummy = ss.str();
        strcpy( errstatus, dummy.c_str() );
        return -1 ;
    }
//
    std::string filename ( ifilename ) ; 
    XmlCfgBRK cfg;
    cfg.loadConfigBrokerConnection( filename );
    cfg.saveConfigBrokerConnection( filename + ".c_out");
//    
    strcpy( ohost, cfg.bc_host.c_str() );
    strcpy( oport, cfg.bc_port.c_str() );
//
    rc = computeLength ( ohost, MAX_HOSTNAME_LENGTH );
    if ( rc == -1) {
        std::stringstream ss ;
        ss << "MAX_HOSTNAME_LENGTH up to " << MAX_HOSTNAME_LENGTH-1 << " characters !! :-) !!" ;
        std::string dummy = ss.str();
        strcpy( errstatus, dummy.c_str() );
        return -1 ;
    }
    rc = computeLength ( oport, MAX_PORT_LENGTH );
    if ( rc == -1) {
        std::stringstream ss ;
        ss << "MAX_PORT_LENGTH up to " << MAX_PORT_LENGTH-1 << " characters !! :-) !!" ;
        std::string dummy = ss.str();
        strcpy( errstatus, dummy.c_str() );
        return -1 ;
    }
    return 0 ;
//
}

uint16_t configQueueCC (const char *ifilename, char *q_prgname, char *q_input, char *q_output,
                        char *q_error, char *q_timeout, char *errstatus ){
//
    memset( q_prgname ,'\0', MAX_PMG_LENGTH );
    memset( q_input   ,'\0', MAX_QUEUENAME_LENGTH );
    memset( q_output  ,'\0', MAX_QUEUENAME_LENGTH );
    memset( q_error   ,'\0', MAX_QUEUENAME_LENGTH );
    memset( q_timeout ,'\0', MAX_QUEUENAME_LENGTH );
    memset( errstatus ,'\0', MAX_ERRSTATUS_LENGTH );
//
    int16_t rc = computeLength ( ifilename, MAX_FILENAME_LENGTH );
    if ( rc == -1) {
        std::stringstream ss ;
        ss << "ifilename up to " << MAX_FILENAME_LENGTH-1 << " characters !! :-) !!" ;
        std::string dummy = ss.str();
        strcpy( errstatus, dummy.c_str() );
        return -1 ; 
    }
//
    string file = string ( ifilename ) ;
    XmlCfgQUE cfg;
    cfg.loadConfigQueue ( file );
    cfg.saveConfigQueue ( file + ".c_out");
//
    strcpy( q_prgname, cfg.q_prgname.c_str() );
    strcpy( q_input,   cfg.q_input.c_str() );
    strcpy( q_output,  cfg.q_output.c_str() );
    strcpy( q_error,   cfg.q_error.c_str() );
    strcpy( q_timeout, cfg.q_timeout.c_str() );
//
    rc = computeLength ( q_prgname, MAX_PMG_LENGTH );
    if ( rc == -1) {
        std::stringstream ss ;
        ss << "q_prgname up to " << MAX_PMG_LENGTH-1 << " characters !! :-) !!" ;
        std::string dummy = ss.str();
        strcpy( errstatus, dummy.c_str() );
        return -1 ;
    }
    rc = computeLength ( q_input, MAX_QUEUENAME_LENGTH );
    if ( rc == -1) {
        std::stringstream ss ;
        ss << "q_input name up to " << MAX_QUEUENAME_LENGTH-1 << " characters !! :-) !!" ;
        std::string dummy = ss.str();
        strcpy( errstatus, dummy.c_str() );
        return -1 ;
    }
    rc = computeLength ( q_output, MAX_QUEUENAME_LENGTH );
    if ( rc == -1) {
        std::stringstream ss ;
        ss << "q_output name up to " << MAX_QUEUENAME_LENGTH-1 << " characters !! :-) !!" ;
        std::string dummy = ss.str();
        strcpy( errstatus, dummy.c_str() );
        return -1 ;
    }
    rc = computeLength ( q_error, MAX_QUEUENAME_LENGTH );
    if ( rc == -1) {
        std::stringstream ss ;
        ss << "q_error name up to " << MAX_QUEUENAME_LENGTH-1 << " characters !! :-) !!" ;
        std::string dummy = ss.str();
        strcpy( errstatus, dummy.c_str() );
        return -1 ;
    }
    rc = computeLength ( q_timeout, MAX_QUEUENAME_LENGTH );
    if ( rc == -1) {
        std::stringstream ss ;
        ss << "q_timeout name up to " << MAX_QUEUENAME_LENGTH-1 << " characters !! :-) !!" ;
        std::string dummy = ss.str();
        strcpy( errstatus, dummy.c_str() );
        return -1 ;
    }
//
    return 0 ;
}

uint16_t configDbConnectionCC ( const char *ifilename, char *odbprovider, char *odbname, char *odbuser, char *odbpassword, char *errstatus )
{
//
    memset( odbprovider   ,'\0', MAX_DBNAME_LENGTH );
    memset( odbname       ,'\0', MAX_DBNAME_LENGTH );
    memset( odbuser       ,'\0', MAX_DBNAME_LENGTH );
    memset( odbpassword   ,'\0', MAX_DBNAME_LENGTH );
    memset( errstatus     ,'\0', MAX_ERRSTATUS_LENGTH );
//
    int16_t rc = computeLength ( ifilename, MAX_FILENAME_LENGTH );
    if ( rc == -1) {
        std::stringstream ss ; 
        ss << "ifilename up to " << MAX_FILENAME_LENGTH-1 << " characters !! :-) !!" ; 
        std:string dummy = ss.str();
        strcpy (  errstatus, dummy.c_str() ) ; 
        return -1 ;
    }
//
    string file = string ( ifilename ) ;
    XmlCfgDB cfg;
    cfg.loadConfigDb ( file );
    cfg.saveConfigDb ( file + ".c_out");
//
    strcpy( odbprovider, cfg.db_provider.c_str() );
    strcpy( odbname, cfg.db_name.c_str() );
    strcpy( odbuser, cfg.db_user.c_str() );
    strcpy( odbpassword, cfg.db_password.c_str() );
//
    rc = computeLength ( odbprovider, MAX_DBNAME_LENGTH );
    if ( rc == -1) {
        std::stringstream ss ;
        ss << "odbprovider name up to " << MAX_DBNAME_LENGTH-1 << " characters !! :-) !!" ;
        std::string dummy = ss.str();
        strcpy (  errstatus, dummy.c_str() ) ;
        return -1 ;
    }
    rc = computeLength ( odbname, MAX_DBNAME_LENGTH );
    if ( rc == -1) {
        std::stringstream ss ;
        ss << "odbname MAX_DBNAME_LENGTH up to " << MAX_DBNAME_LENGTH-1 << " characters !! :-) !!" ;
        std::string dummy = ss.str();
        strcpy (  errstatus, dummy.c_str() ) ;
        return -1 ;
    }
    rc = computeLength ( odbuser, MAX_DBNAME_LENGTH );
    if ( rc == -1) {
        std::stringstream ss ;
        ss << "odbuser up to " << MAX_DBNAME_LENGTH-1 << " characters !! :-) !!" ;
        std::string dummy = ss.str();
        strcpy (  errstatus, dummy.c_str() ) ;
        return -1 ;
    }
    rc = computeLength ( odbpassword, MAX_DBNAME_LENGTH );
    if ( rc == -1) {
        std::stringstream ss ;
        ss << "odbpassword up to " << MAX_DBNAME_LENGTH-1 << " characters !! :-) !!" ;
        std::string dummy = ss.str();
        strcpy (  errstatus, dummy.c_str() ) ;
        return -1 ;
    }
    return 0 ;
//
}
