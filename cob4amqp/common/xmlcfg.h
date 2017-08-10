#ifndef XMLCFG_H
#define XMLCFG_H
/*
 * Source from http://www.boost.org/doc/libs/1_42_0/libs/property_tree/examples/debug_settings.cpp
 */
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/foreach.hpp>
#include <string>
#include <cstring>
#include <set>
#include <exception>
#include <iostream>
#include <sstream>
#include "common.h"
#include "message.h"

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

using namespace std;
//
struct XmlCfgBRK
{
    std::string bc_provider ;
    std::string bc_host ;
    std::string bc_transport ;
    std::string bc_port ;
    std::string bc_user ;
    std::string bc_password ;
//
    void loadConfigBrokerConnection	(const std::string &filename);
    void saveConfigBrokerConnection	(const std::string &filename);
}  ;
//
struct XmlCfgQUE
{
    std::string q_prgname ;
    std::string q_input ;
    std::string q_output ;
    std::string q_error ;
    std::string q_timeout ;
//
    void loadConfigQueue			(const std::string &filename);
    void saveConfigQueue			(const std::string &filename);
}  ;
//
struct XmlCfgDB 
{
    std::string db_provider ;
    std::string db_name ;
    std::string db_user ;
    std::string db_password ;
    std::string db_host ;
    std::string db_port ;
    std::string db_connstr ;
//
    void loadConfigDb				(const std::string &filename);
    void saveConfigDb				(const std::string &filename);
} ;
/*
 * C++ Functions
 */
uint16_t configBrokerConnection ( const std::string &filename, XmlCfgBRK &cfg, std::string &errstatus ) ;
uint16_t configQueue	 		( const std::string &filename, XmlCfgQUE &cfg, std::string &errstatus );
uint16_t configDbConnection		( const std::string &filename, XmlCfgDB  &cfg, std::string &errstatus) ;
/*
 * C Functions
 */
uint16_t configBrokerConnectionCC	( const char *ifilename, char *ohost, char *oport, char *errstatus ) ;
uint16_t configQueueCC	 			( const char *ifilename, char *q_prgname, char *q_input, char *q_output, char *q_error, char *q_timeout, char *errstatus );
uint16_t configDbConnectionCC		( const char *ifilename, char *odbprovider, char *odbname, char *ouser, char *opassword, char *errstatus ) ;

//

#ifdef __cplusplus
}
#endif
//
#endif  /* ! XMLCFG_H */
