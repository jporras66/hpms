package com.indarsoft.hps.server;

import org.apache.commons.configuration.ConfigurationException;
import com.indarsoft.hps.server.bean.ServerBean;
import com.indarsoft.hps.utl.*;

public class ServerFactory {
	
	/**
	 * Create an instance of {@link Server }.
	 * 
	 */
	public static Server getServerInstance( ServerBean serverbean ) {

		Server server = Server.getInstance( serverbean ) ;
		return server ;
	}
	
	/**
	 * Create an instance of {@link Server }.
	 * 
	 */
	public static Server getServerInstance( String serverxmlfile , String brokerxmlfile )  throws ConfigurationException {

		ServerBean serverbean = Config.LoadServerBean( serverxmlfile, brokerxmlfile ) ;
		Server server = Server.getInstance( serverbean ) ;
		return server ;
	}
}
