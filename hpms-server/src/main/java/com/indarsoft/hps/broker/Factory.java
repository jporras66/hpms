package com.indarsoft.hps.broker;

import javax.jms.JMSException;
import org.apache.qpid.jms.JmsSession;
import com.indarsoft.hps.broker.bean.BrokerBean;

public class Factory {
	
	/**
	 * Create an instance of {@link Broker }.
	 * @throws JMSException 
	 * 
	 */
	public static Broker getBrokerInstance ( BrokerBean brokerbean  ) throws JMSException  {
		
		Broker broker= Broker.getInstance(brokerbean);
		return broker ;
	}
	
	/**
	 * Create an instance of {@link BrokerSession }.
	 * @throws JMSException 
	 * 
	 */
	public static BrokerSession getBrokerSessionInstance ( Broker broker ) throws JMSException  {

		BrokerSession brokersession = BrokerSession.getInstance(broker) ;
		return brokersession ;
	}	
	
	/**
	 * Create an not non persistent instance of {@link Producer }. 
	 * @throws JMSException 
	 * 
	 */		
	public static Producer getNotPersistentProducerInstance ( JmsSession session, String queuename ) throws JMSException {
		
		Producer p = Producer.getNonPersistentInstance( session, queuename ) ;
		return p ;	
	}
	
	/**
	 * Create an persistent instance of {@link Producer }. 
	 * @throws JMSException 
	 * 
	 */		
	public static Producer getPersistentProducerInstance ( JmsSession session, String queuename ) throws JMSException {
		
		Producer p = Producer.getPersistentInstance( session, queuename ) ;
		return p ;	
	}
	
	/**
	 * Create an instance of {@link Consumer }. 
	 * @throws JMSException 
	 * 
	 */		
	public static Consumer getConsumerInstance ( JmsSession session, String queuename ) throws JMSException {
		
		Consumer c = Consumer.getInstance( session, queuename ) ;
		return c ;	
	}	
	
	/**
	 * Create an instance of {@link Consumer }. 
	 * @throws JMSException 
	 * 
	 */		
	public static Consumer getConsumerInstance ( JmsSession session, String queuename, String filter ) throws JMSException {
		
		Consumer c = Consumer.getInstance( session, queuename, filter ) ;
		return c ;	
	}		
}
