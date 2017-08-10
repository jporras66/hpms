package com.indarsoft.hps;

import java.util.HashMap;
import javax.jms.JMSException;
import org.apache.log4j.Logger;
import org.apache.qpid.jms.message.JmsBytesMessage;
import org.apache.qpid.jms.message.JmsMessage;
import org.apache.qpid.jms.message.JmsTextMessage;
import org.apache.qpid.jms.provider.amqp.message.AmqpJmsMessageFacade;
import org.apache.qpid.jms.provider.amqp.message.AmqpMessageSupport;
import org.bouncycastle.util.Arrays;
import com.indarsoft.hps.broker.Broker;
import com.indarsoft.hps.broker.Factory;
import com.indarsoft.hps.broker.BrokerSession;
import com.indarsoft.hps.broker.Consumer;
import com.indarsoft.hps.broker.Producer;
import com.indarsoft.hps.broker.bean.BrokerBean;
import com.indarsoft.hps.db.DbBean;
import com.indarsoft.hps.utl.Config;
import com.indarsoft.hps.utl.DbUtl;
import com.indarsoft.hps.utl.MessageUtl;

/*
 * Every received message schedules a timeout:
 * 
 * 		Every received message is written to REQ.SVi.HOST and also saved to REQ.SVi.TIMEOUT .
 * 		A timeout request is written to DB (a new row in TIMEOUT DB table for the messageUUID ).
 * 
 *		When the response is received from HOST, the timeout is removed by response.sh ( res_trx )
 *		( removes the row in TIMEOUT DB table for the messageUUID )
 * 
 * TimeoutMessage listen for messages REQ.SVi.TIMEOUT_i (inputqueue) , with an entry in TIMEOUT DB table :
 * 
 *    - When the timeout is finished an attempt to delete the row from TIMEOUT is executed for the messageUUID.
 *    - In case the delete returns 0 row , the message has been answered from the HOST, and nothing else is necessary.
 *    - In case the delete returns 1 row , the message has NOT been answered from the HOST (which means that has been TIMEOUT), 
 *      and a TIMEOUT response is generated to RES.SVi (outputqueue)
 *    
 */
public class TimeoutMessage implements Runnable {

	private static Logger log = Logger.getLogger(MoveMessage.class.getName());
	private static DbBean dbbean;
	private static BrokerBean brokerbean;
	private static Broker broker;
	private static String timeoutqueue;
	private static String outputqueue;
	private static int timeout;
	private JmsMessage message;

	public static void main(String[] args) throws Exception {
		//
		timeoutqueue  = System.getProperty("inputqueue");
		outputqueue   = System.getProperty("outputqueue");
		timeout = Integer.parseInt(System.getProperty("timeout"));
		//
		String dbxmlfile = System.getProperty("db.config");
		dbbean = Config.LoadDbBean(dbxmlfile);
		//
		String brokerxmlfile = System.getProperty("broker.config");
		brokerbean = Config.LoadBrokerBean(brokerxmlfile);
		broker = Factory.getBrokerInstance(brokerbean);
		BrokerSession consumersession = Factory.getBrokerSessionInstance(broker);
		Consumer consumer = Factory.getConsumerInstance( consumersession.getSession(), timeoutqueue);
		//
		while (true) {
			try {
				JmsMessage msg = consumer.recvMsg();
				Thread th = new Thread(new TimeoutMessage(msg));
				th.start();
			} catch (JMSException e) {
				e.printStackTrace();
			}
		}
	}

	public TimeoutMessage (JmsMessage msg) throws Exception {
		this.message = msg;
	}

	public void run() {

		doIt(this.message);

	}

	public void doIt(JmsMessage inmsg) {
		try {
			/*
			 * Pause execution for timeout millisecond  
			 */
			Thread.sleep( timeout );
			/*
			 * Format as an AMQP message
			 */
			AmqpJmsMessageFacade amqpmsg = (AmqpJmsMessageFacade) inmsg.getFacade();
			/*
			 * Message Properties
			 */
			HashMap<String, Object> applicationproperties = MessageUtl.getApplicationProperties(amqpmsg);
			String messageUUID = "";
			for (String name : applicationproperties.keySet()) {
				log.debug(" ApplicationProperty : " + name + " -> " + applicationproperties.get(name));
				if (name.equals("messageUUID"))
					messageUUID = applicationproperties.get(name).toString();
			}
			//
			if ( DbUtl.removeTimeoutDb( dbbean, messageUUID ) != 0 ){
				/*
				 * Open a new Broker session for every timeout message
				 */
				BrokerSession producersession = Factory.getBrokerSessionInstance( broker );
				Producer producer = Factory.getNotPersistentProducerInstance( producersession.getSession(), outputqueue );
				byte x_opt_jms_msg_type = MessageUtl.getAMQPMessageType(amqpmsg);
				JmsBytesMessage binmsg = null;
				JmsTextMessage textmsg = null;
				if (x_opt_jms_msg_type == AmqpMessageSupport.JMS_TEXT_MESSAGE) {
					textmsg = (JmsTextMessage) inmsg;
					String dummy = "message timeout - " + textmsg.getText() ;
					producer.sendTextMsg( dummy , applicationproperties);
				}
				if (x_opt_jms_msg_type == AmqpMessageSupport.JMS_BYTES_MESSAGE) {
					binmsg = (JmsBytesMessage) inmsg;
					byte[] bar = new byte[(int) binmsg.getBodyLength()];
					binmsg.readBytes(bar);
					producer.sendBinaryMsg( Arrays.concatenate( "message timeout - ".getBytes() , bar ) , applicationproperties );
				}
				producersession.close();
			}
			//

			
		} catch (JMSException | InterruptedException e) {
			log.debug("JMSException" + e.getMessage());
		}
	}
}
