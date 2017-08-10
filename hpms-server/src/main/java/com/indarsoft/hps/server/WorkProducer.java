package com.indarsoft.hps.server;

import java.io.InputStream;
import java.net.Socket;
import java.util.Arrays;
import java.util.HashMap;
import java.util.UUID;

import org.apache.log4j.Logger;
import org.bouncycastle.asn1.ASN1InputStream;
import org.bouncycastle.asn1.ASN1Primitive;
import org.bouncycastle.asn1.DERGeneralString;
import org.bouncycastle.asn1.DEROctetString;

import com.indarsoft.hps.broker.Broker;
import com.indarsoft.hps.broker.Factory;
import com.indarsoft.hps.broker.Producer;
import com.indarsoft.hps.server.bean.ClientId;
import com.indarsoft.utl.Binary;

public class WorkProducer extends Worker {

	private static Logger 	log = Logger.getLogger( WorkProducer.class.getName()  );
	private Producer        producer;
	@SuppressWarnings("unused")
	private int             maxbuff ; 
	
    public WorkProducer ( Socket socket , ClientId clientid, Broker broker , int maxbuff ) throws Exception {
    	super(socket , clientid , broker );
    	Thread.currentThread().setName(  WorkProducer.class.getName()  );
    	this.maxbuff  		= maxbuff ;
    	this.producer 		= Factory.getNotPersistentProducerInstance ( brokersession.getSession() , clientid.getInputQueue() );
    }

    @Override
    public void run() { 
//    	
    	InputStream inputstream  	= null ;
    	ASN1InputStream ans1inputstream = null ;
        try { 
        	inputstream = super.getSocket().getInputStream();
        	ans1inputstream = new ASN1InputStream( inputstream );
            while (true){ 	
//            	
            	ASN1Primitive ans1p = ans1inputstream.readObject();
/*
 * Remove Type/Length from the ANS1 message and move only the octects content from the message, to the byte output array
 */
	            DEROctetString   deros ; 
	            DERGeneralString dergs ; 
	            if ( ans1p instanceof DERGeneralString ){
	            	dergs = (DERGeneralString) ans1p ;
	            	producer.sendTextMsg( dergs.getString() , setApplicationProperties() );
	            	log.debug( "DERGeneralString.sendTextMsg : numbytes -> " + dergs.getString().length() + " -> "+ dergs.getString() );
	            	continue ;
	            }
	            if ( ans1p instanceof DEROctetString ) {
	            	deros = (DEROctetString) ans1p ;
	            	byte barout[] = Arrays.copyOf( deros.getOctets() , deros.getOctets().length ) ;
		        	producer.sendBinaryMsg ( barout , setApplicationProperties() );
		        	log.debug( "DEROctetString.sendBinaryMsg : numbytes -> " + barout.length + " -> "+ Binary.toHexStr( barout ) );
	            	continue ;
	            }
//	            	
		        log.debug("Fatal ERROR - Not an ASN1(DEROctetString/DERGeneralString) message, so discarded : numbytes -> " + ans1p.getEncoded().length + " ->"+ Binary.toPrintableString( ans1p.getEncoded() ) );
		        log.debug("Closing Connection !!! " );
		        break ;
            } 
        } catch ( Exception e ) {        	
        	log.debug( "Exception : " + e.getMessage() );
		}finally{
            try {
            	ans1inputstream.close();
            	inputstream.close();
            	brokersession.close();
			} catch ( Exception e ) {
				log.debug( "Exception : " + e.getMessage() );
			}
		}
    }
    
    public HashMap<String, Object> setApplicationProperties(){
    	HashMap<String, Object> aplicationproperties = new HashMap<String, Object>();
    	aplicationproperties.put( "clientIp",    (String) super.getClientIp() );
    	aplicationproperties.put( "clientPort",  (String) super.getClientPort()  );
    	aplicationproperties.put( "messageUUID", (String) UUID.randomUUID().toString()  ); 	
    	return aplicationproperties ;
    }
}    
