package com.indarsoft.hps.broker;

import javax.jms.JMSException;
import org.apache.qpid.jms.JmsMessageConsumer;
import org.apache.qpid.jms.JmsQueue;
import org.apache.qpid.jms.JmsSession;
import org.apache.qpid.jms.message.JmsBytesMessage;
import org.apache.qpid.jms.message.JmsMessage;
import org.apache.qpid.jms.message.JmsTextMessage;

public class Consumer {
	
	private JmsSession 			session ;
	private JmsQueue		 	queue;
	private JmsMessageConsumer 	consumer;
	
	private Consumer ( JmsSession s , String queuename  ) throws JMSException  {
		this.session = s ;
		queue 	 = (JmsQueue) session.createQueue( queuename );
        consumer = (JmsMessageConsumer) session.createConsumer( queue );
	}
	
	private Consumer ( JmsSession s , String queuename, String selector  ) throws JMSException {
		this.session = s ;
		//String selector = "FromRemote = '" + filter +"'" ;
		queue 	 = (JmsQueue) session.createQueue( queuename );
        consumer = (JmsMessageConsumer) session.createConsumer( queue, selector );
	}
	
	protected static Consumer getInstance ( JmsSession s, String queuename  ) throws JMSException {
		return new Consumer ( s , queuename );
	}
	
	protected static Consumer getInstance ( JmsSession s, String queuename, String filter  ) throws JMSException {
		return new Consumer ( s , queuename , filter );
	}
			

	
	public JmsBytesMessage recvBinaryMsg () throws JMSException {
		JmsBytesMessage message = (JmsBytesMessage) consumer.receive();
        return message ;
	}	
	
	public JmsTextMessage recvTxtMsg () throws JMSException {
		JmsTextMessage message = (JmsTextMessage) consumer.receive();
        return message ;
	}	
	
	public JmsMessage recvMsg () throws JMSException {
		JmsMessage message =  (JmsMessage) consumer.receive();
        return message ;
	}
	
	public JmsSession getSession() {
		return session;
	}

	public JmsQueue getQueue() {
		return queue;
	}

	public JmsMessageConsumer getConsumer() {
		return consumer;
	}

}
