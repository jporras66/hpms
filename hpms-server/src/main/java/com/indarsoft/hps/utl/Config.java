package com.indarsoft.hps.utl;

import java.util.HashMap;
import java.util.List;

import org.apache.commons.configuration.ConfigurationException;
import org.apache.commons.configuration.XMLConfiguration;
import org.apache.commons.configuration.HierarchicalConfiguration;
import org.apache.log4j.Logger;

import com.indarsoft.hps.broker.bean.BrokerBean;
import com.indarsoft.hps.broker.bean.BrokerQueue;
import com.indarsoft.hps.db.DbBean;
import com.indarsoft.hps.server.bean.ClientId;
import com.indarsoft.hps.server.bean.ServerBean;
import com.indarsoft.hps.server.bean.Clients;

public class Config {

	static Logger 		 	log = Logger.getLogger( Config.class.getName() );
	
	public static BrokerBean LoadBrokerBean ( String brokerxmlfile ) throws ConfigurationException  { 
		
		XMLConfiguration config = new XMLConfiguration( brokerxmlfile );
		String provider		= config.getString("BrokerConnection.provider");
		String host 		= config.getString("BrokerConnection.host");
		String transport 	= config.getString("BrokerConnection.transport");
		String port			= config.getString("BrokerConnection.port");
		String user			= config.getString("BrokerConnection.user");
		String password		= config.getString("BrokerConnection.password");
		String dispatchinputqueue 	= config.getString("ClientDispatcher.dispatchInputQueue");
		String dispatchoutputqueue  = config.getString("ClientDispatcher.dispatchOutputQueue");
		BrokerQueue brokerqueue  = new BrokerQueue ( dispatchinputqueue, dispatchoutputqueue ) ;
		BrokerBean  brokerbean   = new BrokerBean ( provider, host, transport, port, user, password, brokerqueue );
		return brokerbean;
	}
	     
	public static ServerBean LoadServerBean ( String serverxmlfile , String brokerxmlfile) throws ConfigurationException {
//
		XMLConfiguration config = new XMLConfiguration( serverxmlfile );
		String name     = config.getString("Server.name");
		String address  = config.getString("Server.address");
		int port        = config.getInt("Server.port");
		int maxbuff     = config.getInt("Server.maxBuffer");
		boolean acl     = config.getBoolean("Server.acl");
//
		HashMap< String, ClientId > hmclientId  = new HashMap< String, ClientId >();
		List< HierarchicalConfiguration > list_clients = config.configurationsAt("clientId");	
		for( HierarchicalConfiguration c : list_clients) {
			String ip 					= c.getString( "ip" );
			String inputqueue 			= c.getString( "inputQueue" );
			String outputqueue 			= c.getString( "outputQueue" );
			int    maxconnections 	    = c.getInt( "maxConnections" ) ;
			ClientId clientBean = new ClientId ( ip, inputqueue, outputqueue, maxconnections  );
			hmclientId.put( ip , clientBean) ;
		}
		Clients clients = Clients.get( hmclientId ) ;
		log.debug(  clients.toString() );
//
		BrokerBean    brokerbean    = LoadBrokerBean ( brokerxmlfile ) ; 
		ServerBean    serverbean    = new ServerBean ( name, address, port , maxbuff, acl, clients , brokerbean );
		
		return serverbean;
     }
//
	public static DbBean LoadDbBean ( String dbxmlfile ) throws ConfigurationException  { 
		
		XMLConfiguration config = new XMLConfiguration( dbxmlfile );
		String provider		= config.getString("DbConnection.provider");
		String host 		= config.getString("DbConnection.hostname");
		String port			= config.getString("DbConnection.port");
		String dbname		= config.getString("DbConnection.dbname");
		String user			= config.getString("DbConnection.user");
		String password		= config.getString("DbConnection.password");
		DbBean dbbean   	= new DbBean ( provider, host, port, dbname, user, password );
		return dbbean;
	}
//
	public static String getAplName ( ) {
		
		return "hps-server" ;
	}
}
