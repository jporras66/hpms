package com.indarsoft.hps.client;

import org.apache.commons.configuration.ConfigurationException;
import org.apache.commons.configuration.XMLConfiguration;

public class LoadConfig {

	
	public static ClientBean Client ( String xmlfile ) throws ConfigurationException {	
		XMLConfiguration config = new XMLConfiguration( xmlfile );
		String host 			= config.getString("ServerConnection.host");
		int port				= config.getInt("ServerConnection.port");
		int maxBuffer  			= config.getInt("ServerConnection.maxBuffer");
		int maxInnerQueueLength = config.getInt("maxInnerQueueLength");
		String clientName 		= config.getString("clientName");
		ClientBean cb = new ClientBean( host, port, maxBuffer, maxInnerQueueLength , clientName );
		return cb;
	}
	
	public static String getAplName ( ) {
		
		return "hps-client" ;
	}

}
