package com.indarsoft.hps.broker.bean;

public class BrokerBean {

    private String provider	; 
    private String host 	;  
	private String transport;
    private String port 	;
	private String user 	;
    private String password ;
//
	private BrokerQueue brokerqueue ;
//

	public BrokerBean ( String provider, String host, String transport, String port, String user, String password, BrokerQueue brokerqueue ) {
		this.provider 		= provider;	
		this.transport 		= transport;
		this.host			= host;
		this.port 			= port;
		this.user			= user;
		this.password		= password;
		this.brokerqueue    = brokerqueue ; 
	}


	public String getProvider() {
		return provider;
	}

	public String getHost() {
		return host;
	}

	public String getTransport() {
		return transport;
	}

	public String getPort() {
		return port;
	}
    
	public String getUser() {
		return user;
	}

	public String getPassword() {
		return password;
	}

    public void setHost(String host) {
		this.host = host;
	}

	@Override
	public String toString() {
		return  "BrokerBean [provider=" + provider + ", host=" + host +
				", transport=" + transport + ", port=" + port + ", user=" + user + 
				", password=" + password + brokerqueue.toString() + "]";
	}
}
