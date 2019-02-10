package com.indarsoft.hps.server.bean;

import com.indarsoft.hps.broker.bean.BrokerBean;

/*	
 * name :  		Server name
 * address :  	IP where hpms-server is running
 * port :  		Port where the sockect is listening
 * maxbuff :  	Max message buffer length 
 * acl :  		true  -> Check if the client trying to connect is in the clientId's list
 *				false -> Any client is allowed to connect
 * clients :	List of clients allowed to connect
 * brokerbean :	Broker connections details
 *  
 */

public class ServerBean {

	private String 			name ;
	private String 			address ;
	private int 			port ;
	private int 			maxbuff;
	private boolean			acl;
	private Clients 		clients ; 
	private BrokerBean		brokerbean ; 

	public ServerBean( String name, String address, int port , int maxbuff , boolean acl , Clients clients , BrokerBean brokerbean){
		this.name			= name ;
		this.address		= address ;
		this.port 			= port ;
		this.maxbuff		= maxbuff ;
		this.acl 			= acl ; 
		this.clients		= clients ;
		this.brokerbean     = brokerbean ;
	}
	
	public int getPort() {
		return port;
	}


	public String getName() {
		return name ;
	}

	public String getAddress() {
		return address ;
	}
	
	public Clients getClients() {
		return clients;
	}
	
	public BrokerBean getBrokerBean() {
		return brokerbean;
	}

	public int getMaxbuff() {
		return maxbuff;
	}
	
	public boolean getIsAclEnabled() {
		return acl;
	}
	public  void updateClientId ( String key , ClientId cid) {
		clients.updateEntry( key, cid );
	}
	
	@Override
	public String toString() {
		return "ServerBean [name=" + name  + ", address=" + address + ", port=" + port + ", maxbuff=" + maxbuff +  brokerbean.toString() + "]";
	}
	

}
