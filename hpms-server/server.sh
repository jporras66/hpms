#!/bin/bash
# =======================================================================================
# 
#  Server Main process  
#
#  Server Configuration	: 	-D server.config
#  Broker Configuration : 	-D broker.config
#
# =======================================================================================
#
#export JAVA_HOME=/usr/java/jdk1.8.0_71
export JRE=$JAVA_HOME/jre
export JARFILES=./hpms-server-1.0.0.jar:./lib/*
#
$JRE/bin/java -Dserver.config=./config/server_properties.xml -Dbroker.config=./config/broker_properties.xml -cp  $JARFILES com.indarsoft.hps.ServerMain
#