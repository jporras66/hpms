package com.indarsoft.hps.db;

public class DbBean {

    private String provider	; 
    private String hostname 	;  
    private String port 	;
    private String dbname ;
	private String user 	;
    private String password ;

//

	public DbBean ( String provider, String hostname, String port, String dbname , String user, String password ) {
		this.provider 		= provider;	
		this.hostname		= hostname;
		this.port 			= port;
		this.dbname			= dbname;
		this.user			= user;
		this.password		= password;
	}


	public String getProvider() {
		return provider;
	}

	public String getHostname() {
		return hostname;
	}

	public String getPort() {
		return port;
	}
	
	public String getDbname() {
		return  dbname;
	}
    
	public String getUser() {
		return user;
	}

	public String getPassword() {
		return password;
	}

    public void setHost(String host) {
		this.hostname = host;
	}

	@Override
	public String toString() {
		return  "BrokerBean [ " + 
	            "provider=" + provider + 
				", host=" + hostname +
				", port=" + port + 
				", dbname=" + dbname + 
				", user=" + user + 
				", password=" + password  + "]";
	}
}
