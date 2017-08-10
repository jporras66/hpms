package com.indarsoft.hps.server;

import java.net.Socket;
import javax.jms.JMSException;
import com.indarsoft.hps.broker.Broker;
import com.indarsoft.hps.broker.Factory;
import com.indarsoft.hps.broker.BrokerSession;
import com.indarsoft.hps.server.bean.ClientId;

public class Worker implements Runnable {

	private 	Socket 			socket ;
	private 	ClientId 		clientid;
	protected 	Broker		 	broker;		
	protected 	BrokerSession 	brokersession;	
//	
	private long 		threadid ;
	private String 		clientIp	;
	private String 		clientPort ;

	public Worker ( Socket socket ,  ClientId 	clientid , Broker broker ) throws JMSException {
        this.socket   = socket;
        this.clientid = clientid;
        this.setRemote() ;
        threadid 	   = Thread.currentThread().getId() ;
//
    	this.brokersession 	= Factory.getBrokerSessionInstance( broker );    	
//    	
    }

    public void run() {

    }  
    
    public ClientId getClientId() {
		return clientid;
	}
    
    public void setRemote(){	
		String str = socket.getRemoteSocketAddress().toString();
		String dummy = str.substring(1);
		String [] data = dummy.split(":");
		clientIp 	= data[0];
		clientPort	= data[1];
    }
    
	public Socket getSocket() {
		return socket;
	}

	public long getThreadid() {
		return threadid;
	}

	public String getClientIp() {
		return clientIp;
	}

	public String getClientPort() {
		return clientPort;
	}
	
	public Broker getBroker() {
		return broker;
	}

	public BrokerSession getBrokersession() {
		return brokersession;
	}	
        
}    
