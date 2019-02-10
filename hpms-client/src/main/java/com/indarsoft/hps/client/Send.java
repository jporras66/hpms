package com.indarsoft.hps.client;

import java.io.OutputStream;
import java.io.IOException;
import java.net.Socket;
import java.net.SocketException;
import java.util.Arrays;
import java.util.concurrent.BlockingQueue;
import org.apache.log4j.Logger;
import org.bouncycastle.asn1.ASN1OutputStream;
import org.bouncycastle.asn1.ASN1Primitive;
/*import com.indarsoft.utl.Binary;*/
import org.bouncycastle.asn1.DERGeneralString;
import org.bouncycastle.asn1.DEROctetString;

import com.indarsoft.hps.examples.asn1.Asn1Structure;
import com.indarsoft.utl.Binary;

@SuppressWarnings("unused")
public class Send implements Runnable{

	private static Logger  			log = Logger.getLogger( Send.class.getName() );
	private Socket      			socket ;
	private BlockingQueue<byte[]> 	queuesnd ;
	private ClientBean configbean ; 
	
    public Send (Socket s , BlockingQueue<byte[]> q , ClientBean cb ) throws SocketException {
    	this.socket      = s ; 
        this.queuesnd    = q ;
        this.configbean  = cb ;
        this.socket.setTcpNoDelay(true);
    }

    public void run() {
        OutputStream output = null ;
        ASN1OutputStream ans1outputstream = null ;
        try { 
            output = socket.getOutputStream();
        	ans1outputstream = new ASN1OutputStream( output );
            while (true){
            	String attach = configbean.getClientName() + " : " ;
            	byte[] bar = queuesnd.take() ;	//dequeue
/*
 * Send Text Message : 0x05 + String strout.getBytes()
 */	            
	            if ( bar[0] == 0x05   ){
	            	String srtout = attach + Binary.toPrintableString( Arrays.copyOfRange( bar , 1, bar.length ) ) ;
	    		    log.debug( "0x05-Send Text Message 	 - length : " + srtout.length() + " data -> " + srtout );
	    		    DERGeneralString dergs = new DERGeneralString ( srtout );
	            	ans1outputstream.writeObject( dergs );
	            	ans1outputstream.flush();
	            	continue ;
	            }
/*
 * Send Binary Message : 0x03 + byte[] bar
 */		            
	            if ( bar[0] == 0x03 ) {
	    		    log.debug( "0x03-Send Binary Message - length : "+  bar.length + " data -> "+ Binary.toHexStr( Arrays.copyOfRange( bar , 1, bar.length ) ) );
	    		    byte[] barout = org.bouncycastle.util.Arrays.concatenate( attach.getBytes() , Arrays.copyOfRange( bar , 1, bar.length ) ) ;
	    		    DEROctetString deros = new DEROctetString (  barout )  ;
	            	ans1outputstream.writeObject( deros );
	            	ans1outputstream.flush();  
	            	continue ;
	            }
//            	
	            log.debug( "ERROR - Not an ASN1(DEROctetString/DERGeneralString) message, so discarded  -> " + Binary.toPrintableString( bar ) );	            
            }    
        } catch (IOException e) {
        	log.debug( "IOException : " + e.getMessage() );
        } catch (InterruptedException e) {
        	log.debug( "InterruptedException : " + e.getMessage() );
		}finally{
            try {
            	ans1outputstream.close();
				output.close();
			} catch (IOException e) {
				e.printStackTrace();
			}
        }
    }  
}    

