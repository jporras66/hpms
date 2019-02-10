package com.indarsoft.hps.utl;

import java.sql.Connection;
import java.sql.DriverManager;
import java.sql.PreparedStatement;
import java.sql.SQLException;

import org.apache.log4j.Logger;

import com.indarsoft.hps.db.DbBean;

public class DbUtl {

	private static Logger  log = Logger.getLogger( DbUtl.class.getName() );

	@SuppressWarnings("finally")
	public static int removeTimeoutDb( DbBean dbbean, String uuid ) {

		Connection dbconn = null;
		String driver = "org." + dbbean.getProvider() + ".Driver";
		String dburl = "jdbc:" + dbbean.getProvider() + "://"
				+ dbbean.getHostname() + ":" + dbbean.getPort() + "/"
				+ dbbean.getDbname();
		try {
			Class.forName(driver);
			dbconn = DriverManager.getConnection(dburl, dbbean.getUser(),
					dbbean.getPassword());
			dbconn.setAutoCommit(false);
			PreparedStatement deleteTimeoutPrstm = null;
			String deleteString = "delete from timeout where uuid = ? ";
			deleteTimeoutPrstm = dbconn.prepareStatement(deleteString);
			deleteTimeoutPrstm.setString(1, uuid);
			int rows = deleteTimeoutPrstm.executeUpdate();
			if (rows == 0) {
				log.debug("timeout for message uuid  " + uuid
						+ " has no entry in timeout table !! ");
			} else {
				log.debug("timeout for message uuid  " + uuid
						+ " has been deleted");
			}
			dbconn.commit();
			deleteTimeoutPrstm.close();
			dbconn.close();
			return rows ;
		} catch (Exception e) {
			e.printStackTrace();
			log.debug(e.getClass().getName() + " : " + e.getMessage());
			if (dbconn != null) {
				try {
					System.err.print("Transaction is being rolled back");
					dbconn.rollback();
				} catch (SQLException ex) {
					ex.printStackTrace();
				}finally{
					return 0 ;
				}
			}
			else {
				return 0 ;
			}
		}
	}
}
