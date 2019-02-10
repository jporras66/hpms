package com.indarsoft.hps.utl;

import static org.junit.Assert.*;
import org.apache.commons.configuration.ConfigurationException;
import org.junit.Test;
import com.indarsoft.hps.broker.bean.BrokerBean;
import com.indarsoft.hps.utl.Config;

public class ConfigTest {

	public String serxmlfile    = "server_properties.xml" ;
	public String clixmlfile    = "client_properties.xml" ;	
	public String brokerxmlfile = "broker_properties.xml" ;
//	

	@Test
	public void LoadBrokerBean() throws ConfigurationException {

		BrokerBean bb = Config.LoadBrokerBean( brokerxmlfile ) ;
		if (  bb.getProvider().equals("QPID") ) {
			System.out.println("LoadBrokerBean  provider : TRUE - " + bb.getProvider()  ) ; 
			System.out.println("LoadBrokerBean  provider : TRUE - " + bb.toString()  ) ;
			assertTrue( true) ;
		}else{
			if (  bb.getProvider().equals("ACTIVEMQ") ) {
				System.out.println("LoadBrokerBean  provider : TRUE - " + bb.getProvider()  ) ; 
				System.out.println("LoadBrokerBean  provider : TRUE - " + bb.toString()  ) ;
				assertTrue( true) ;				
			}else{
				System.out.println("LoadBrokerBean  provider : FALSE - " + bb.getProvider()  ) ; 
			}
		}
	}	
}
