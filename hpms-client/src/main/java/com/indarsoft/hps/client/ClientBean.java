package com.indarsoft.hps.client;

	
public class ClientBean {

/*
	host :  				Node name or IP where hpms-server is running
	port :  				Node port where the sockect is listening
	maxBuffer :  			Max message buffer length 
	maxInnerQueueLength :  	Number of defaults message to be created in a internal queue (BlockingQueue) for asynchronous client send/receive
	clientName :			Client name 
*/
	
	private String 	host ;
	private int 	port ;
	private int 	maxInnerQueueLength ; 
	private int 	maxBuffer  ; 
	private String  clientName ;
	
	public ClientBean( String host, int port, int maxBuff, int maxInn, String clientName ){
		this.host					= host ;
		this.port 					= port ;
		this.maxInnerQueueLength 	= maxInn ; 
		this.maxBuffer 				= maxBuff ; 
		this.clientName 			= clientName ; 
	}
	public String getHost() {
		return host;
	}

	public int getPort() {
		return port;
	}

	public int getMaxInnerQueueLength() {
		return maxInnerQueueLength;
	}	

	public int getMaxBufferMsgLength() {
		return maxBuffer;
	}

	public String getClientName() {
		return clientName;
	}
	
	@Override
	public String toString() {
		return "ConfigBean [client.Name=" + clientName + ",client.maxInnerQueueLength=" + maxInnerQueueLength + ",ServerConnection.hostname=" + host + ",ServerConnection.port=" + port + ",ServerConnection.maxBuffer=" + maxBuffer + "]" ;
	}
}
