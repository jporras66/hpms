#!/bin/bash
# =======================================================================================
#
# This process simulates a response from HOST 
# 
# Move messages from 	: 	-D inputqueue=REQ.SV1.HOST 
# to 					: 	-D outputqueue=RES.SV1.HOST or RES.SV1.C1 ( in case you want all the responses in the same queue ) 
# delay time			:	-D delay=2000
# 
# The message is delayed between 1000 to (delay + 1000) milliseconds, so messages written to outputqueue ARE NOT 
# IN THE SAME ORDER than received messages in inputqueue
#
# ========================================================================================
#
#export JAVA_HOME=/usr/java/jdk1.8.0_71
export JRE=$JAVA_HOME/jre
export JARFILES=./hpms-server-1.0.0.jar:./lib/*
#
# timeout : amount in milliseconds ( i.e.  2000 is 2 seconds )
#
$JRE/bin/java -Dbroker.config=./config/broker_properties.xml -Dinputqueue=REQ.SV1.HOST -Doutputqueue=RES.SV1.C1 -Ddelay=2000 -cp  $JARFILES com.indarsoft.hps.MoveMessage
#