package com.indarsoft.hps.utl;

import java.util.ArrayList;
import java.util.Collections;

/**
* An abstract class implementing the Runnable interface that can be used to notify listeners
* when the runnable thread has completed. To use this class, first extend it and implement
* the doRun function - the doRun function is where all work should be performed. Register any listeners that 
* want to be notified, Then create a new thread with this as its Runnable. 
*
*/

public abstract class NotificationRunnable implements Runnable{

	/**
	 * An abstract function that children must implement. This function is where 
	 * all work - typically placed in the run of runnable - should be placed. 
	 */
	public abstract void doRun();

	/**
	 * Our list of listeners to be notified upon thread completion.
	 */
	private java.util.List<ThreadListener> listeners = Collections.synchronizedList( new ArrayList<ThreadListener>() );

	/**
	 * Adds a listener to this object. 
	 * @param listener Adds a new listener to this object. 
	 */
	public void addListener( ThreadListener listener ){
		listeners.add(listener);
	}
	/**
	 * Removes a particular listener from this object, or does nothing if the listener
	 * is not registered. 
	 * @param listener The listener to remove. 
	 */
	public void removeListener( ThreadListener listener ){
		listeners.remove(listener);
	}
	/**
	 * Notifies all listeners that the thread has completed.
	 */
	private final void notifyListeners() {
		synchronized ( listeners ){
			for (ThreadListener listener : listeners) {
			  listener.notifyOfCompletion(this);
			}
		}
	}
	/**
	 * Implementation of the Runnable interface. This function first calls doRun(), then
	 * notifies all listeners of completion.
	 */
	public void run(){
		try{
			doRun();
		}finally{
			notifyListeners();
		}
	}
}