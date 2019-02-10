package com.indarsoft.hps.utl;

import java.util.Enumeration;
import java.util.HashMap;
import java.util.Set;

import javax.jms.JMSException;

import org.apache.log4j.Logger;
import org.apache.qpid.jms.message.*;
import org.apache.qpid.jms.provider.amqp.message.*;

import com.indarsoft.utl.Binary;

public class MessageUtl {

	private static Logger  log = Logger.getLogger( MessageUtl.class.getName() );
	
	public static HashMap<String, Object> getApplicationProperties ( AmqpJmsMessageFacade amqpmsg ) throws JMSException{
		Set<String> set = amqpmsg.getPropertyNames() ;
    	HashMap<String, Object> applicationproperties = new HashMap<String, Object>();
		for ( String name  : set ){
			Object o = amqpmsg.getApplicationProperty( name ) ;
			String value2display ="" ; 
			String dummy =  "Application Properties type -> name-->value : " ;
			if ( o instanceof String ){
				String value = (String) amqpmsg.getApplicationProperty( name ) ;
				applicationproperties.put( name.replace("-", "_") , value);
				value2display = value ; 
				dummy =  dummy  + " String -> ";
			}
			if ( o instanceof Integer ){
				Integer value = (Integer) amqpmsg.getApplicationProperty( name ) ;
				applicationproperties.put( name.replace("-", "_") , value);
				value2display = Integer.toString( value );
				dummy =  dummy  + " Integer -> ";
			}
			if ( o instanceof Long ){
				Long value = (Long) amqpmsg.getApplicationProperty( name ) ;
				applicationproperties.put( name.replace("-", "_") , value);
				value2display = Long.toString( value );
				dummy =  dummy  + " Long   --> ";
			}			
			dummy =  dummy  + name.replace("-", "_") +" --> "+value2display ;
			//System.out.println( dummy ); 
    		log.debug( dummy );
		}
		return applicationproperties ;
	}
	
	public static byte getAMQPMessageType ( AmqpJmsMessageFacade amqpmsg ) throws JMSException{
		String key = "x-opt-jms-msg-type";
		Object object = AmqpMessageSupport.getMessageAnnotation( key , amqpmsg.getAmqpMessage() );
		return (byte) object ;
	}
	
    @SuppressWarnings({ "unchecked", "unused" })
	public static void displayJmsMessageProperties ( JmsMessage msg ) throws JMSException{
/*
 * Defined JMS Properties        		
 */
		Enumeration<String> e = (Enumeration<String>)   msg.getAllPropertyNames() ;
		while(e.hasMoreElements()){
			String name = e.nextElement();
			String dummy = "All properties name : " + name ;
			System.out.println( dummy ); 
			log.debug( dummy  );
		}
/*
 * Not null JMS Properties        		
 */		
		Enumeration<String> f = (Enumeration<String>)   msg.getAllPropertyNames() ;
		while( f.hasMoreElements()){
			String name = f.nextElement();
			boolean b = msg.propertyExists( name  ) ;
			if ( b ) {
				String dummy = "JMS property exists : " + name ;
				System.out.println( dummy ); 
				log.debug( dummy );			
			}
		}
//
		AmqpJmsMessageFacade amqpmsg = (AmqpJmsMessageFacade) msg.getFacade() ;
/*
 * Application Properties
 */
		HashMap<String, Object> hm = getApplicationProperties ( amqpmsg ) ;		
/*
 * AMQP message annotations    		
 */
		String key = "x-opt-jms-msg-type";
		Object object = AmqpMessageSupport.getMessageAnnotation( key , amqpmsg.getAmqpMessage() );
		if ( object != null){
			byte x_opt_jms_msg_type = (byte) object ; 
			String dummy = "AMQP Message Annotation : " + key + " is byte value 0x"+Binary.toHexStr( (byte) object );
			System.out.println( dummy ); 
			log.debug( dummy );			
		}
//
		key = "x-opt-to-type";
		object = AmqpMessageSupport.getMessageAnnotation( key , amqpmsg.getAmqpMessage() );
		if ( object != null){
			String x_opt_to_type = (String) object ; 
			String dummy = "AMQP Message Annotation : " + key + "      is string value "+ x_opt_to_type ;
			System.out.println( dummy ); 
			log.debug( dummy );			
		}
//		
		key = "x-opt-jms-dest";
		object = AmqpMessageSupport.getMessageAnnotation( key , amqpmsg.getAmqpMessage() );
		if ( object != null){
			String dummy = "AMQP Message Annotation : " + key + "     is byte value 0x"+Binary.toHexStr( (byte) object );
			System.out.println( dummy ); 
			log.debug( dummy );			
		}else{
			String dummy = "AMQP Message Annotation : " + key + "     NOT available" ;
			System.out.println( dummy ); 
			log.debug( dummy );				
		}
//      		
    }	
}
