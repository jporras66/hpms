package com.indarsoft.hps.broker;

import java.util.HashMap;
import javax.jms.DeliveryMode;
import javax.jms.JMSException;
import org.apache.qpid.jms.JmsMessageProducer;
import org.apache.qpid.jms.JmsQueue;
import org.apache.qpid.jms.JmsSession;
import org.apache.qpid.jms.message.JmsBytesMessage;
import org.apache.qpid.jms.message.JmsTextMessage;


public class Producer {
	
	private JmsSession 			session ;
	private JmsQueue		 	queue;
	private JmsMessageProducer 	producer;
	
	private Producer ( JmsSession s , String queuename, int persistency ) throws JMSException {
		this.session = s ;
		queue 	 = (JmsQueue) session.createQueue( queuename );
        producer = (JmsMessageProducer) session.createProducer( queue );
        producer.setDeliveryMode( persistency  );
	}
	
	protected static Producer getNonPersistentInstance ( JmsSession s, String queuename  ) throws JMSException {
		return new Producer ( s , queuename , DeliveryMode.NON_PERSISTENT );
	}
	
	protected static Producer getPersistentInstance ( JmsSession s, String queuename  ) throws JMSException {
		return new Producer ( s , queuename , DeliveryMode.PERSISTENT );
	}
	
	public void sendTextMsg ( String txt ) throws JMSException {
		JmsTextMessage message =  (JmsTextMessage) session.createTextMessage( txt );
        producer.send( message );
	}

	public void sendTextMsg ( String txt , String property , Object value ) throws JMSException {
		JmsTextMessage message =  (JmsTextMessage) session.createTextMessage( txt );
		message.setObjectProperty( property , value  );
        producer.send( message );
	}
	
	public void sendTextMsg ( String txt  , HashMap<String, Object> hmp ) throws JMSException {
		JmsTextMessage message =  (JmsTextMessage) session.createTextMessage( txt );
		for( String key: hmp.keySet() ){
			message.setObjectProperty( key , hmp.get(key)  );
		}
        producer.send( message );
	}		

	public void sendBinaryMsg ( byte[] b ) throws JMSException {
		JmsBytesMessage message = (JmsBytesMessage) session.createBytesMessage();
		message.writeBytes( b );
        producer.send( message );
	}
	

	public void sendBinaryMsg ( byte[] b , String property , Object value ) throws JMSException {
		JmsBytesMessage message =  (JmsBytesMessage) session.createBytesMessage();
		message.setObjectProperty( property , value  );
		message.writeBytes( b );
        producer.send( message );
	}
	
	public void sendBinaryMsg ( byte[] b , HashMap<String, Object> hmp ) throws JMSException {
		JmsBytesMessage message = (JmsBytesMessage) session.createBytesMessage();
		for( String key: hmp.keySet() ){
			message.setObjectProperty( key , hmp.get(key)  );
		}
		message.writeBytes( b );
        producer.send( message );
	}	
}
