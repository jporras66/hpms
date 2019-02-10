package com.indarsoft.hps.broker;

import javax.jms.JMSException;
import org.apache.qpid.jms.JmsSession;

/*
 * AUTO_ACKNOWLEDGE
 * With this acknowledgment mode, the session automatically acknowledges a client's receipt of a message 
 * either when the session has successfully returned from a call to receive or when the message listener 
 * the session has called to process the message successfully returns.
 */
public class BrokerSession {
	
	private Broker      	broker ;
	private JmsSession 		session;
	private boolean 		isTransacted = false ;
	
	private BrokerSession ( Broker broker ) throws JMSException {
		this.broker		= broker ;
        this.session = (JmsSession) broker.getConnection().createSession( isTransacted , JmsSession.AUTO_ACKNOWLEDGE);
	}
	
	protected static BrokerSession getInstance ( Broker broker ) throws JMSException {
		return new BrokerSession ( broker  );
	}

	public Broker getBroker() {
		return broker;
	}

	public JmsSession getSession() {
		return session;
	}
    
	public void close() throws JMSException{
		session.close();
	}
	public boolean isTransacted() {
		return isTransacted;
	}
}
