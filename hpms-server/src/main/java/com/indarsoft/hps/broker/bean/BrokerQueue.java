package com.indarsoft.hps.broker.bean;

public class BrokerQueue {
	
	private String 		dispatchinputqueue ;
	private String 		dispatchoutputqueue ;
	
	public BrokerQueue ( String dispatchinputqueue , String dispatchoutputqueue  ) {
		this.dispatchinputqueue  = dispatchinputqueue ; 
		this.dispatchoutputqueue = dispatchoutputqueue ;
	}

	public String getDispatchinputqueue() {
		return dispatchinputqueue;
	}

	public String getDispatchoutputqueue() {
		return dispatchoutputqueue;
	}
	
	@Override
	public String toString() {
		return "BrokerQueue [ dispatchinputqueue=" + dispatchinputqueue + ",dispatchoutputqueue=" + dispatchoutputqueue + "]";
	}
}
