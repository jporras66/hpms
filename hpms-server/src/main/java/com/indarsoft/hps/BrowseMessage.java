package com.indarsoft.hps;

import java.util.Enumeration;

import javax.jms.Message;

import org.apache.qpid.jms.message.JmsMessage;
import org.apache.qpid.jms.JmsQueueBrowser;

import com.indarsoft.hps.broker.Broker;
import com.indarsoft.hps.broker.BrokerSession;
import com.indarsoft.hps.broker.Factory;
import com.indarsoft.hps.broker.bean.BrokerBean;
import com.indarsoft.hps.utl.Config;
import com.indarsoft.hps.utl.MessageUtl;

import org.apache.qpid.jms.JmsQueue;

public class BrowseMessage {

	private static String 	brokerxmlfile ;	
	private String 			inputqueue ;
	
	public static void main(String[] args) throws Exception {

		BrowseMessage msgbrs = new BrowseMessage();
		msgbrs.inputqueue  = System.getProperty("inputqueue");
		//String messageSelector = "( clientIp='127.0.0.1' ) AND ( clientPort='56194' )";				
		//String messageSelector = "( clientIp='127.0.0.1' )"  ;
		String messageSelector = System.getProperty("messageSelector");  ;
//
		brokerxmlfile = System.getProperty("broker.config");
		BrokerBean brokerbean 			= Config.LoadBrokerBean( brokerxmlfile ) ;
		Broker broker					= Factory.getBrokerInstance( brokerbean ) ;
		BrokerSession brokersession 	= Factory.getBrokerSessionInstance( broker );
//		
		JmsQueue jmsqueue = (JmsQueue) brokersession.getSession().createQueue( msgbrs.inputqueue );
		JmsQueueBrowser jmsbrowser    = (JmsQueueBrowser) brokersession.getSession().createBrowser( jmsqueue, messageSelector );
		
		Enumeration<Message> msgs = jmsbrowser.getEnumeration();
		//
		if ( !msgs.hasMoreElements() ) { 
		    System.out.println("No messages in queue");
		    //return ;
		} else { 
		    while (msgs.hasMoreElements()) { 
		    	JmsMessage tempMsg = (JmsMessage)msgs.nextElement(); 
		        System.out.println("Message: " + tempMsg); 
		        MessageUtl.displayJmsMessageProperties( tempMsg );
		    }
		}
//
		brokersession.close();
		broker.connectionClose();
//		
		System.out.println("Exiting ...");
	}

}
