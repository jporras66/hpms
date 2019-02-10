package com.indarsoft.hps.server.bean;

import java.util.HashMap;

public class Clients {
	
	private HashMap< String, ClientId > hm = new HashMap< String, ClientId > () ;
	
	private Clients ( HashMap< String, ClientId > h ) {
		for ( String key : h.keySet() ){
			hm.put (key, h.get(key) );
		}
	}
	
	public static Clients get ( HashMap< String, ClientId > h ){
		return new Clients ( h ) ; 
	}
	
	public HashMap<String, ClientId> getHashmap() {
		return hm;
	}
	
	public  ClientId getClientId ( String key ) {
		return hm.get( key );
	}
	
	public  void updateEntry ( String key , ClientId clientid) {
		hm.put (key, clientid );
	}

	@Override
	public String toString() {
		String s = "Clients [ " ;
		for ( String key : hm.keySet() ){
			String dummy  = "\n IP=%15s %s";
			String result = String.format( dummy , key , hm.get(key).toString() ) ;
			s = s + result ;
		}
		return  s + "\n " + "]" ;
	}
}
