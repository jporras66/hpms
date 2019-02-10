package com.indarsoft.hps.server.bean;

public class ClientId {

	private String ip ; 
	private String inputqueue ;
	private String outputqueue ;
	private int    maxconnections ;
	private int    curconnections = 0 ;	


	public ClientId( String ip, String inputqueue, String outputqueue, int maxconnections ){
		
		this.ip 				 = ip;
		this.inputqueue			 = inputqueue;
		this.outputqueue		 = outputqueue;
		this.maxconnections      = maxconnections ;
	}
	
	public String getIp() {
		return ip;
	}
	public String getInputQueue() {
		return inputqueue;
	}

	public String getOutputQueue() {
		return outputqueue;
	}
	
	public int getMaxConnections() {
		return maxconnections;
	}
	
	public void updateConnections() {
		curconnections ++ ;
	}

	public boolean verifyLimitConnections() {
		int val = curconnections ;
		val ++ ;
		if ( val > maxconnections ) return false ;
		return true ;
	}
	@Override
	public String toString(){
		String dummy  = "  inputqueue=%26s  outputqueue=%26s  maxconnections=%2s";
		String result = String.format ( dummy, inputqueue, outputqueue, maxconnections );
		return result ;	
	}
	
}
