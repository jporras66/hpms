package com.indarsoft.hps;

import java.net.Socket;
import java.util.Scanner;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import org.apache.log4j.Logger;
import com.indarsoft.hps.client.Recv;
import com.indarsoft.hps.client.Send;
import com.indarsoft.hps.client.ClientBean;
import com.indarsoft.hps.client.LoadConfig;

public class ClientMain {

	static Logger log = Logger.getLogger( ClientMain.class.getName() );
	public static String xmlfile ;
	public static int threadsNumber ;
	public static int messagesNumber ;
	public static int maxInnQl ; 
//
	private Socket socket ; 
//	
	public static void main(String[] args) throws Exception {

		xmlfile = System.getProperty("client.config");
		ClientBean clientbean  = LoadConfig.Client ( xmlfile ) ;
		threadsNumber	= Integer.valueOf( System.getProperty("threads.number") );
		messagesNumber 	= Integer.valueOf( System.getProperty("messages.number") );
//		
		String dummy = "Starting Client threads.number : " + threadsNumber + " messages.number  : " + messagesNumber;
		System.out.println ( dummy ) ; 
		log.debug( dummy );
		System.out.println ( clientbean.toString()  ) ; 
		log.debug( clientbean.toString()  );
		dummy = "                                 "  ;
		System.out.println ( dummy ) ; 
		log.debug( dummy );
/*
 * Queues for Load to Client communication
 * 
 */
		maxInnQl  = clientbean.getMaxInnerQueueLength();		
		BlockingQueue<byte[]> queuercv = new ArrayBlockingQueue<byte[]>( maxInnQl );
		BlockingQueue<byte[]> queuesnd = new ArrayBlockingQueue<byte[]>( maxInnQl );
/*
 * Starting client Services ( Send & Recv per client connection ) 
 * 		
 */
		ClientMain clientmain = new ClientMain ();
		clientmain.startServices( clientbean , queuercv , queuesnd ); 
//		
		dummy = "Finishing Client threads.number : " + threadsNumber + " messages.number  : " + messagesNumber ;
		System.out.println ( dummy ) ; 
		log.debug( dummy );		
	}
//
	private void startServices( ClientBean clientbean, BlockingQueue<byte[]> queuercv, BlockingQueue<byte[]> queuesnd )  {
//
		try {
			socket = new Socket( clientbean.getHost () , clientbean.getPort()  ) ;
/*
 * receiver & sender threads
 */       			
           Thread recv = new Thread( new Recv ( socket ,  queuercv , clientbean ) );
           recv.start();                 			
           Thread sender = new Thread( new Send ( socket ,  queuesnd , clientbean ) );
           sender.start();    
/*
 * RecvLoad - Only one thread to receive messages through BlockingQueue<byte[]> qrcv
 * SendLoad 
 */			
       		ExecutorService recvpool = Executors.newSingleThreadExecutor();
            Runnable rl = new RecvLoad ( queuercv , clientbean.getClientName() );
            recvpool.execute( rl );
//
            Scanner in = new Scanner(System.in); 
       		ExecutorService sendpool = Executors.newFixedThreadPool ( threadsNumber );
       		String response = "" ; 
       		while(! response.equalsIgnoreCase( "end" ) ){
       			System.out.println("Sending " + messagesNumber + " messages of type text & " + messagesNumber + " messages of type binary ...");
       			startSendLoad  ( clientbean, queuesnd, sendpool ) ;
           		System.out.println("Type end to finish sending messages .. ");
           		response = in.nextLine();
       		}
       		in.close();
       		System.out.println(".... end Typed ....end ");
/* 
 * This will make the executor accept no new threads and finish all existing threads in the queue
 * Wait until all threads are finish
 * 
 */		
/*			sendpool.shutdown();
			sendpool.awaitTermination( 2 , TimeUnit.SECONDS ) ;
			recvpool.shutdown();
			recvpool.awaitTermination( 2 , TimeUnit.SECONDS ) ;
			sender.join();
			recv.join();*/
//			
		} catch ( Exception e) {
			log.debug( "Exception : " + e.getMessage() );
		} 
	}
/*
 * 	SendLoad - Pool with numTh workers
 *  NOTE : To be modified to sendpool.invokeAll instead of sendpool.execute ( only 1 thread ) 
 *  
 */	
	private void startSendLoad ( ClientBean clientbean, BlockingQueue<byte[]> queuesnd , ExecutorService sendpool)  {
//
		try { 

       		for (int i = 0; i < messagesNumber ; i++) {
       			Runnable sl = new SendLoad ( queuesnd, clientbean.getClientName() );
                sendpool.execute( sl );
            }		
		} catch ( Exception e) {
			log.debug( "Exception : " + e.getMessage() );
		} 
	}	
	
}
