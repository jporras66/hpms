package com.indarsoft.hps.utl;

/**
 * A interface that can be used by the NotificationThread class to notify an
 * object that a thread has completed. 
 * 
 */
public interface ThreadListener {
	/**
	 * Notifies this object that the Runnable object has completed its work. 
	 * @param runner The runnable interface whose work has finished.
	 */
	public void notifyOfCompletion( Runnable runner );
}