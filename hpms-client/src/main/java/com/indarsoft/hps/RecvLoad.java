package com.indarsoft.hps;

import java.io.File;
import java.util.concurrent.BlockingQueue;
import org.apache.log4j.Logger;

import com.indarsoft.hps.client.Send;
import com.indarsoft.utl.Utl;

public class RecvLoad implements Runnable {
	
	private static Logger  log = Logger.getLogger( Send.class.getName() );
	private String sep 			= File.separator;
	private String pwd 			= Utl.getPwd() ;
	private String filename 	= pwd + sep + "data" + sep + "received" + sep ;
	private String clientname	;
//
	private BlockingQueue<byte[]> rcvblockingqueue ;
	
	protected RecvLoad( BlockingQueue<byte[]> q , String clientname ){
		this.rcvblockingqueue 	= q ;
		this.clientname			= clientname ;
    	Thread.currentThread().setName(  Send.class.getName()  );
	}
	public void run(){
		try { 
			String filenameout = ""; 
			while ( true ){
				byte barout[] = rcvblockingqueue.take() ;
//
				long current = System.nanoTime();
				filenameout = filename + clientname+"-"+current  + ".qrcv";
            	Utl.writeBinary( filenameout, barout );        
			}    
	    } catch ( InterruptedException e) {
	      log.debug( "InterruptedException : " + e.getMessage() );
	    }
	}
}
