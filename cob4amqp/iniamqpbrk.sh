#
export CAPLIC=cob4amqp
#
#
# Please edit the value for APL to point to the right directory
#
export APL=$HOME/apl/cob4amqp
#
#
# OpenCobol
# 
# COB_PRE_LOAD : is an environment variable that controls what dynamic link modules are included in a run.
# COB_LIBRARY_PATH  :  To find <program>.so --> cobcrun <program>.so
#
echo  "start CONFIG VARIABLES ... "
echo  "==========================="
export OC=/opt/opencobol
export OCESQL=/opt/ocesql
echo  "OpenCobol installed in " $OC
echo  "Ocesql installed in    " $OCESQL
echo  "Apache ActiveMQ installed in /opt/activemq-5.14.4 "
#
export QPID_HOME=/opt/qpid-broker/6.0.4
export QPID_WORK=/opt/qpidwork
echo  "Apache QPID installed in " $QPID_HOME
echo  "Apache QPID work dir in  " $QPID_WORK
#
export COBCPY=$APL/copy:$OCESQL/copy:$COBCPY
#
export COB_PRE_LOAD=$OCESQL/lib/libocesql.so
export COB_LIBRARY_PATH=$APL/lib:$OCESQL/lib
#
export LD_LIBRARY_PATH=$OC/lib:$OCESQL/lib:$APL/lib:$LD_LIBRARY_PATH
export LD_RUN_PATH=$OC/lib:$OCESQL/lib:$APL/lib:$LD_RUN_PATH
export PATH=$OC/bin:$OCESQL/bin:/bin:$PATH
#
# to see what the AMQP protocol level interactions are
#
#export PN_TRACE_FRM=1
#
export RESOURCES=$APL/resources/
export BROKERCONNECTION=$APL/resources/broker_connection.xml
export DBCONNECTION=$APL/resources/db_connection.xml
export FILEDAT=$RESOURCES/filedat.out
echo  "==========================="
echo  "end   CONFIG VARIABLES ... "
#
cd $APL
