package com.indarsoft.hps.broker;

import javax.jms.JMSException;
import org.apache.qpid.jms.JmsConnection;
import org.apache.qpid.jms.JmsConnectionFactory;
import com.indarsoft.hps.broker.bean.BrokerBean;

public class Broker {

	private BrokerBean  	brokerbean ;
	private JmsConnection 	connection ;
	
	private Broker ( BrokerBean brokerbean ) throws JMSException {
		this.brokerbean = brokerbean ;
		String uri = brokerbean.getTransport().toLowerCase() +"://"+ brokerbean.getHost() + ":" + brokerbean.getPort();
		JmsConnectionFactory factory = new JmsConnectionFactory(  uri );
		this.connection = (JmsConnection) factory.createConnection ( brokerbean.getUser(), brokerbean.getPassword() ) ;
		this.connection.start();		
	}

	protected static Broker getInstance ( BrokerBean brokerbean ) throws JMSException {
		
		Broker brk = new Broker ( brokerbean ) ;
	    return brk;
	}

	public BrokerBean getBrokerBean() {
		return brokerbean;
	}
	
    public JmsConnection getConnection () {
		return connection;

	}

	public void connectionClose() throws JMSException {
    	connection.close();
    }

}
