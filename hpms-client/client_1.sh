#!/bin/bash
# ========================================================================================================
# 
#  Client for HPMS Server
#
#  Client Configuration	: 	-D client.config
#
# ========================================================================================================
#
#export JAVA_HOME=/usr/java/jdk1.8.0_71
export JRE=$JAVA_HOME/jre
export JARFILES=./hpms-client-1.0.0.jar:./lib/*
#
$JRE/bin/java -Dclient.config=./config/client_properties_1.xml -Dthreads.number=5 -Dmessages.number=50  -cp  $JARFILES  com.indarsoft.hps.ClientMain
#
  
