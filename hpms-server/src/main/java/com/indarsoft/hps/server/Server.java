package com.indarsoft.hps.server;

import java.net.InetAddress;
import java.net.ServerSocket;
import java.net.Socket;
import java.io.IOException;

import org.apache.log4j.Logger; 

import com.indarsoft.hps.broker.Broker;
import com.indarsoft.hps.broker.Factory;
import com.indarsoft.hps.server.bean.ClientId;
import com.indarsoft.hps.server.bean.ServerBean;

public class Server implements Runnable  {

	static Logger 		 	log = Logger.getLogger( Server.class.getName() );
    private ServerSocket	serversocket = null; 
    private ServerBean 		serverbean ;
    private InetAddress		serveraddress = null ; 
	private InetAddress		remoteaddress = null ; 
	//    
	private Server( ServerBean serverbean  ){
		this.serverbean = serverbean ;
    }
	
	public static Server getInstance ( ServerBean serverbean  ){
		return new Server ( serverbean ) ;
	}

	public ServerBean getServerbean() {
		return serverbean;
	}
	
	public InetAddress getRemoteaddress() {
		return remoteaddress;
	}

    public InetAddress getServeraddress() {
		return serveraddress;
	}
    
    public void run() {   
        try {
        	this.serveraddress = InetAddress.getByName( serverbean.getAddress() );
            this.serversocket  = new ServerSocket( serverbean.getPort() );
        } catch (IOException e) {
            String msg = "Cannot open port " + serverbean.getPort() + " - " + e.getMessage() ;
            log.debug(  msg );
            throw new RuntimeException( msg, e);
        }
        
        while( true ){
            Socket clientsocket = null;
            ClientId clientid   = null ; 
            try {
                clientsocket = this.serversocket.accept();
                this.remoteaddress = clientsocket.getInetAddress() ;             
                String ip = remoteaddress.getHostAddress() ;
                clientid = serverbean.getClients().getClientId( ip ) ;
//              
                if ( clientid == null  ) {
                    String msg = "Remote client IP " + ip + " is not defined as a valid client !!! :-(" ;
                    log.debug(  msg );
                    clientsocket.close();      	
                }else{
                    Thread wout=null; 
                    Thread win=null;
        			try {
        				Broker broker = Factory.getBrokerInstance( serverbean.getBrokerBean() ) ;
        	            wout = new Thread( new WorkConsumer ( clientsocket, clientid , broker , serverbean.getMaxbuff() ) );
        	            wout.start();
        				win = new Thread( new WorkProducer ( clientsocket,  clientid , broker , serverbean.getMaxbuff() ) );
        	            win.start();
        			} catch ( Exception e ) {
        	            log.debug( e.getMessage() );
        	            throw new RuntimeException( e.getMessage() , e);
        			} 
                }             
            } catch (IOException e) {	            	
                String msg = ".Error accepting client connection" ;
                log.debug( e.getMessage() );
                throw new RuntimeException( msg , e);
            }
        }
    }
}