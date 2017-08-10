#!/bin/bash
# ========================================================================================================
# 
#  Broese messages from a Queue
#
#  Broker Properties 	: 	-D broker.config=broker_properties.xml 
#  Queue to Browse 		: 	-D inputqueue=REQ.SV1.HOST 
#  message selector		: 	-D messageSelector="( clientIp='127.0.0.1' ) AND ( clientPort='56194' )" 
#
# ========================================================================================================
#
#export JAVA_HOME=/usr/java/jdk1.8.0_71
export JRE=$JAVA_HOME/jre
export JARFILES=./hpms-server-1.0.0.jar:./lib/*
#
$JRE/bin/java -Dbroker.config=broker_properties.xml -Dinputqueue=REQ.SV1.HOST -DmessageSelector="( clientIp='127.0.0.1' ) AND ( clientPort='56194' )" -cp  $JARFILES com.indarsoft.hps.BrowseMessage
#