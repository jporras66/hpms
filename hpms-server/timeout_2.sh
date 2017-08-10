#!/bin/bash
# =======================================================================================
# 
# Manages timeout messages :
#
# 		Every received message is written to REQ.SVi.HOST and also saved to REQ.SVi.TIMEOUT .
# 		A timeout request is written to DB (a new row in TIMEOUT DB table for the messageUUID ).
# 
#		When the response is received from HOST, the timeout is removed by response.sh ( res_trx )
#		( removes the row in TIMEOUT DB table for the messageUUID )
# 
# TimeoutMessage listen for messages REQ.SVi.TIMEOUT_i (inputqueue) , with an entry in TIMEOUT DB table :
# 
#    - When the timeout is finished an attempt to delete the row from TIMEOUT is executed for the messageUUID.
#    - In case the delete returns 0 row , the message has been answered from the HOST, and nothing else is necessary.
#    - In case the delete returns 1 row , the message has NOT been answered from the HOST (which means that has been TIMEOUT), 
#      and a TIMEOUT response is generated to RES.SVi (outputqueue)
#    
#
#
# ========================================================================================
#
# timeout : amount in milliseconds ( aka 2000 is 2 seconds )
#
#export JAVA_HOME=/usr/java/jdk1.8.0_71
export JRE=$JAVA_HOME/jre
export JARFILES=./hpms-server-1.0.0.jar:./lib/*
#
# timeout : amount in milliseconds ( aka 2000 is 2 seconds )
#
$JRE/bin/java -Dbroker.config=broker_properties.xml -Dinputqueue=REQ.SV1.TIMEOUT_2  -Doutputqueue=RES.SV1 -Dtimeout=2000 -Ddb.config=pgbouncer_properties.xml -cp  $JARFILES com.indarsoft.hps.TimeoutMessage
#