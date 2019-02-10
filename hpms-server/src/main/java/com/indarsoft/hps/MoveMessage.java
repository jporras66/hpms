package com.indarsoft.hps;

import java.util.HashMap;
import java.util.Random;

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
import com.indarsoft.hps.utl.Config;
import com.indarsoft.hps.utl.MessageUtl;

/*
 * This process simulates a response from HOST 
 * 
 * Move messages from 	: 	-D inputqueue=REQ.SV1.HOST 
 * to 					: 	-D outputqueue=RES.SV1.HOST or RES.SV1.C1 ( in case you want all the responses in the same queue ) 
 * delay time			:	-D delay=2000
 * 
 * Every dequeued message is delayed a random amount of time  between 1000 to 1000 + delay milliseconds 
 * ( 1 to 3 seconds in case delay is 2000), to allow  that some messages would be timeout ( timeoutMessage.java ) 
 * 
 * Messages written to outputqueue ARE NOT IN THE SAME ORDER than received messages in inputqueue
 * 
 */
public class MoveMessage implements Runnable {

	private static Logger log = Logger.getLogger(MoveMessage.class.getName());
	private static BrokerBean brokerbean;
	private static Broker broker;
	private static String inputqueue;
	private static String outputqueue;
	private static int delay;
	private JmsMessage message;

	public static void main(String[] args) throws Exception {
		//
		inputqueue = System.getProperty("inputqueue");
		outputqueue = System.getProperty("outputqueue");
		delay = Integer.parseInt(System.getProperty("delay"));
		//
		String brokerxmlfile = System.getProperty("broker.config");
		brokerbean = Config.LoadBrokerBean(brokerxmlfile);
		broker = Factory.getBrokerInstance(brokerbean);
		BrokerSession consumersession = Factory.getBrokerSessionInstance(broker);
		Consumer consumer = Factory.getConsumerInstance( consumersession.getSession(), inputqueue );
		//
		while (true) {
			try {
				JmsMessage msg = consumer.recvMsg();
				Thread th = new Thread(new MoveMessage(msg));
				th.start();
			} catch (JMSException e) {
				e.printStackTrace();
			}
		}
	}

	public MoveMessage(JmsMessage msg) throws Exception {
		this.message = msg;
	}

	public void run() {

		doIt(this.message);

	}

	public void doIt(JmsMessage inmsg) {
		try {
			/*
			 * ramdom number: between 1000 to (timeout + 1000) milliseconds
			 */
			int min = 1000;
			int max = delay + 1000;
			Random rand = new Random();
			int millisDelay = rand.nextInt(max + 1 - min) + min;
			Thread.sleep(millisDelay);
			/*
			 * Open a new Broker session for every message
			 */
			BrokerSession producersession = Factory
					.getBrokerSessionInstance(broker);
			Producer producer = Factory.getNotPersistentProducerInstance(
					producersession.getSession(), outputqueue);
			/*
			 * Format as an AMQP message
			 */
			AmqpJmsMessageFacade amqpmsg = (AmqpJmsMessageFacade) inmsg
					.getFacade();
			/*
			 * Message Properties
			 */
			HashMap<String, Object> applicationproperties = MessageUtl.getApplicationProperties(amqpmsg);
			for (String name : applicationproperties.keySet()) {
				log.debug(" ApplicationProperty : " + name + " -> " + applicationproperties.get(name));
			}
			//
			byte x_opt_jms_msg_type = MessageUtl.getAMQPMessageType(amqpmsg);
			JmsBytesMessage binmsg = null;
			JmsTextMessage textmsg = null;
			String dummy = "message received from : " +  inputqueue + " to : " + outputqueue + " delayed  " + millisDelay + " - ";
			if (x_opt_jms_msg_type == AmqpMessageSupport.JMS_TEXT_MESSAGE) {
				textmsg = (JmsTextMessage) inmsg;
				producer.sendTextMsg( dummy + textmsg.getText() , applicationproperties);
			}
			if (x_opt_jms_msg_type == AmqpMessageSupport.JMS_BYTES_MESSAGE) {
				binmsg = (JmsBytesMessage) inmsg;
				byte[] bar = new byte[(int) binmsg.getBodyLength()];
				binmsg.readBytes(bar);
				producer.sendBinaryMsg( Arrays.concatenate( dummy.getBytes() , bar ) , applicationproperties );
				//
			}
			producersession.close();
		} catch (JMSException | InterruptedException e) {
			log.debug("JMSException" + e.getMessage());
		}
	}
}
