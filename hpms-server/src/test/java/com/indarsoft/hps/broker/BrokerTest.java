package com.indarsoft.hps.broker;


import static org.junit.Assert.*;

import java.io.File;

import javax.jms.JMSException;

import org.apache.commons.configuration.ConfigurationException;
import org.junit.Test;

import com.indarsoft.hps.broker.bean.BrokerBean;
import com.indarsoft.hps.server.bean.ClientId;
import com.indarsoft.hps.utl.Config;
import com.indarsoft.utl.Ascii;
import com.indarsoft.utl.Utl;

@SuppressWarnings("unused")
public class BrokerTest {
	
	String brkxmlfile = "broker_properties.xml";
	BrokerBean     brokerbean ;
	Broker		   broker ;
	BrokerSession  brokersession ;

	
	@Test
	public void testStart() throws Exception {
//
		try {
			brokerbean 		= Config.LoadBrokerBean( brkxmlfile );
			broker 			= Factory.getBrokerInstance( brokerbean ) ;
			brokersession 	= Factory.getBrokerSessionInstance ( broker );
		} catch (ConfigurationException e) {
			e.printStackTrace();
		}
//		
		if ( brokersession != null   ){
			Producer pro = Factory.getNotPersistentProducerInstance( brokersession.getSession()  , "TEST.PLUS" );
			pro.sendTextMsg("HOLA MUNDO !!!");
			brokersession.close();
			broker.connectionClose();
			assertTrue( true) ;
		}else{
			System.out.println( "FALSE - brk for Provider " + brokerbean.getProvider()+ " is null !! " ) ; 
			assertFalse( true) ;
		}

	}

}
