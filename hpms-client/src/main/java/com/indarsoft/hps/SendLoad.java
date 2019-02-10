package com.indarsoft.hps;

import java.io.File;
import java.io.IOException;
import java.util.List;
import java.util.concurrent.BlockingQueue;
import org.apache.log4j.Logger;
import org.bouncycastle.asn1.DERGeneralString;
import org.bouncycastle.asn1.DEROctetString;
import com.indarsoft.utl.Binary;
import com.indarsoft.utl.Utl;

@SuppressWarnings("unused")
public class SendLoad implements Runnable {
	
	private static Logger  log = Logger.getLogger( SendLoad.class.getName() );
//
	private String sep 				= File.separator;
	private String workingdirectory = Utl.getPwd() ;
	private String txtfilename 	    = workingdirectory + sep+"data"+ sep + "2send" + sep + "text_msg.txt";
	private String binfilename 	    = workingdirectory + sep+"data"+ sep + "2send" + sep + "withdrawal_chip_request.hex";
//
	private byte[] bar ;	
	private String strout ;				
	private BlockingQueue<byte[]> sndblockingqueue ;
	private String clientname	;
//	
	protected SendLoad( BlockingQueue<byte[]> bqueue , String clientname ) throws Exception {
    	Thread.currentThread().setName(  SendLoad.class.getName()  );
		this.sndblockingqueue 	= bqueue ;
		this.clientname			= clientname ;
		this.bar 				= Utl.loadBinary( binfilename ) ;
		List<String> lines 		= Utl.loadFile( txtfilename ) ;
		StringBuilder sb = new StringBuilder() ;
		for (String line : lines) {
			sb.append(line);
		}
		strout = sb.toString();
	}
	public void run(){
		try { 	
/*
 * Send Text Message : 0x05 + String strout.getBytes()
 */
			byte[] msgtypeTxt = { 0x05 } ;
			byte[] btxt = org.bouncycastle.util.Arrays.concatenate( msgtypeTxt , clientname.getBytes() ) ;
			btxt = org.bouncycastle.util.Arrays.concatenate( msgtypeTxt , strout.getBytes() ) ;
			sndblockingqueue.put( btxt ) ;
/*
 * Send Binary Message : 0x03 + byte[] bar
 */		    
			byte[] msgtypeBinary = { 0x03 } ;
			byte[] b = org.bouncycastle.util.Arrays.concatenate( msgtypeBinary , clientname.getBytes()  ) ;
			b = org.bouncycastle.util.Arrays.concatenate( msgtypeBinary , bar ) ;
			sndblockingqueue.put( b ) ;
//    
	    } catch ( InterruptedException  e) {
	      log.debug( "InterruptedException : " + e.getMessage() );
	    }
	}
}
