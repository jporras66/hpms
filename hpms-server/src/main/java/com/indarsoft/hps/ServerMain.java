package com.indarsoft.hps;

import org.apache.commons.configuration.ConfigurationException;

import com.indarsoft.hps.server.Server;
import com.indarsoft.hps.server.ServerFactory;

public class ServerMain {

	static String serverxmlfile ;
	static String brokerxmlfile ;
	
	public static void main(String[] args) throws ConfigurationException {
		
		serverxmlfile = System.getProperty("server.config"); 
		brokerxmlfile = System.getProperty("broker.config"); 
		Server 	server 	 = ServerFactory.getServerInstance ( serverxmlfile, brokerxmlfile ); 
		Thread th=null;
		th = new Thread( server ) ;
		th.start();
		
		try {
			th.join();
		} catch (InterruptedException e) {
			e.printStackTrace();
		}
	}
}
