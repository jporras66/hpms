package com.indarsoft.hps.client;

import java.io.InputStream;
import java.net.Socket;
import java.util.Arrays;
import java.util.concurrent.BlockingQueue;
import org.apache.log4j.Logger;
import org.bouncycastle.asn1.ASN1InputStream;
import org.bouncycastle.asn1.ASN1Primitive;
import org.bouncycastle.asn1.DERGeneralString;
import org.bouncycastle.asn1.DEROctetString;

import com.indarsoft.utl.Binary;

public class Recv implements Runnable{

	private static Logger  log = Logger.getLogger( Recv.class.getName() );
	private Socket      socket ;
	private BlockingQueue<byte[]> 	queuercv ;
	@SuppressWarnings("unused")
	private ClientBean configbean ; 
	
    public Recv (Socket s,  BlockingQueue<byte[]> q ,  ClientBean cb ) {
    	this.socket      = s ;
        this.queuercv    = q ;
        this.configbean  = cb ;
    }

    public void run()  {
    	InputStream 	inputstream  		= null ;
    	ASN1InputStream ans1inputstream = null ;
        try { 
	        inputstream  = socket.getInputStream();
        	ans1inputstream = new ASN1InputStream( inputstream );
	        while (true){ 	
/*	            byte barin[] = new byte[ maxbuff ]; 	
	            int  k = inputstream.read ( barin, 0, maxbuff ) ;
	            if ( k == -1 ) break ;  	
		       	queuercv.put( barin ); */ 
            	ASN1Primitive ans1p = ans1inputstream.readObject();
/*
 * Remove Type/Length from the ANS1 message and move only the octects content from the message, to the byte output array
 */
	            DEROctetString   deros ; 
	            DERGeneralString dergs ; 
	            if ( ans1p instanceof DERGeneralString ){
	            	dergs = (DERGeneralString) ans1p ;
	            	String  strmsg = dergs.getString() ;
	            	log.debug( "Recv Text Message   - length : " + strmsg.length() + " data -> "+ strmsg );
	            	queuercv.put( strmsg.getBytes() ); 
	            	continue ;
	            }
	            if ( ans1p instanceof DEROctetString ) {
	            	deros = (DEROctetString) ans1p ;
	            	byte barout[] = Arrays.copyOf( deros.getOctets() , deros.getOctets().length ) ;
		        	log.debug( "Recv Binary Message - length : " + barout.length + " data -> "+ Binary.toHexStr( barout ) );
	            	queuercv.put( barout ); 
	            	continue ;
	            }            	
//            	
	        log.debug( "Fatal ERROR - Not an ASN1(DEROctetString/DERGeneralString) message, so discarded : numbytes -> " + ans1p.getEncoded().length + " ->"+ Binary.toPrintableString( ans1p.getEncoded() ) );
	        log.debug( "Closing Connection !!! " );
	        break ;
	        }    
        } catch ( Exception e ) {
        	log.debug( "Exception : " + e.getMessage() );
		}finally{
            try {
            	ans1inputstream.close();
	            inputstream.close();
			} catch ( Exception  e) {
				log.debug( "Exception : " + e.getMessage() ) ;
			}
        }
    }  
}      


