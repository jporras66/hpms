package com.indarsoft.hps.server;

import java.io.OutputStream;
import java.io.IOException;
import java.net.Socket;

import javax.jms.JMSException;

import org.apache.log4j.Logger;
import org.apache.qpid.jms.message.JmsBytesMessage;
import org.apache.qpid.jms.message.JmsMessage;
import org.apache.qpid.jms.message.JmsTextMessage;
import org.apache.qpid.jms.provider.amqp.message.AmqpJmsMessageFacade;
import org.apache.qpid.jms.provider.amqp.message.AmqpMessageSupport;
import org.bouncycastle.asn1.ASN1OutputStream;
import org.bouncycastle.asn1.DERGeneralString;
import org.bouncycastle.asn1.DEROctetString;
import org.bouncycastle.util.Arrays;

import com.indarsoft.hps.broker.Broker;
import com.indarsoft.hps.broker.Factory;
import com.indarsoft.hps.broker.Consumer;
import com.indarsoft.hps.server.bean.ClientId;
import com.indarsoft.hps.utl.MessageUtl;
import com.indarsoft.utl.Binary;

@SuppressWarnings("unused")
public class WorkConsumer extends Worker {

	private static Logger log = Logger.getLogger( WorkConsumer.class.getName() );
	private int      	  maxbuff ; 
	private Consumer 	  consumer;
	private String 	 	  filter; 
	
    public WorkConsumer ( Socket socket , ClientId clientid, Broker broker, int maxbuff ) throws Exception {    	
    	super( socket , clientid , broker );
    	Thread.currentThread().setName(  WorkConsumer.class.getName()  );
    	this.maxbuff = maxbuff ;
/*
 * Consume only messages that fits the filter criteria
 */
    	this.filter = "( clientIp='"+super.getClientIp()+"' ) AND ( clientPort='"+super.getClientPort()+"' )";
    	log.debug( "filter : " + filter );
    	consumer = Factory.getConsumerInstance ( brokersession.getSession(), clientid.getOutputQueue(), filter );
    }

    public void run() {
        OutputStream output = null ;  
        ASN1OutputStream ans1outputstream = null ;
        try { 
    		output = super.getSocket().getOutputStream();
        	ans1outputstream = new ASN1OutputStream( output );
            while (true){
/*
 * Property apl_message_type is an application property for message type .
 */
	            JmsMessage inmsg = consumer.recvMsg() ;
				AmqpJmsMessageFacade amqpmsg = (AmqpJmsMessageFacade) inmsg.getFacade() ;
				String apl_message_type = inmsg.getStringProperty( "apl_message_type" );
				String prefix = filter + "->" ;
/*
 * if ( xoptJmsMsgType == AmqpMessageSupport.JMS_TEXT_MESSAGE )
 * if ( x_opt_jms_msg_type == AmqpMessageSupport.JMS_BYTES_MESSAGE )
 * Verify if this AMQP message annotation is defined	"x-opt-jms-msg-type"
 * byte xoptJmsMsgType = MessageUtl.getAMQPMessageType( amqpmsg ) ;
 * 
 */ 				
/*
 * Message encapsulated in a JmsBytesMessage
 * 			
 */
				if ( inmsg instanceof JmsBytesMessage  ){							
					JmsBytesMessage binmsg = (JmsBytesMessage) inmsg ;
		        	byte[] bar = new byte[(int) binmsg.getBodyLength()];
		        	binmsg.readBytes( bar );
/*
 * Text message encapsulated in a JmsBytesMessage
 */
					if ( apl_message_type.equals("5") ){
						String strout = prefix + new String( bar );
						DERGeneralString generalstring  = new DERGeneralString ( strout );
						ans1outputstream.writeObject( generalstring );
						ans1outputstream.flush();
		            	log.debug( "JmsBytesMessage-sendTextMsg : numbytes -> " + strout.length() + " -> "+ strout );
		            	continue ;							
					}
/*
 * Binary Message encapsulated in a JmsBytesMessage				
 */					
					if ( apl_message_type.equals("3") ){
						byte[] barout = Arrays.concatenate( prefix.getBytes(), bar );
			            DEROctetString   octectstring  = new DEROctetString ( barout );
						ans1outputstream.writeObject( octectstring );
						ans1outputstream.flush();
		            	log.debug( "JmsBytesMessage-sendBinaryMsg : numbytes -> " + bar.length + " -> "+ Binary.toHexStr(bar) );
		            	continue ;						
					}							
				}
/*
 * Text message encapsulated in a JmsTextMessage
 * 			
 */				
				if ( inmsg instanceof JmsTextMessage ){
					JmsTextMessage txtmsg = (JmsTextMessage) inmsg ;
					String strout = prefix + txtmsg.getText() ;
					DERGeneralString generalstring  = new DERGeneralString ( strout );
					ans1outputstream.writeObject( generalstring );
					ans1outputstream.flush();
	            	log.debug( "JmsTextMessage-sendTextMsg : numbytes -> " + strout.length() + " -> "+ strout );
	            	continue ;					
				}      				
            }    
        } catch ( Exception e ) {
        	log.debug( "Exception : " + e.getMessage() );
		}finally{
            try {
            	ans1outputstream.close();
				output.close();
				brokersession.close();
			} catch ( IOException | JMSException e  ) {
				log.debug( "Exception : " + e.getMessage() );
			}
        }
    }  
 
}    

