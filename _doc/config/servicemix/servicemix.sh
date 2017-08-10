#
export JAVA_HOME=/usr/java/jdk1.7.0_79
export JAVA_MIN_MEM=128M # Minimum memory for the JVM
export JAVA_MAX_MEM=1024M # Maximum memory for the JVM
export JAVA_PERM_MEM=128M # Minimum perm memory for the JVM
export JAVA_MAX_PERM_MEM=256M # Maximum perm memory for the JVM
#
export SERVICEMIX_HOME=/opt/servicemix-7.0.0.M2
export SVMX_SERVER=$SERVICEMIX_HOME
export PATH=$PATH:$JAVA_HOME/bin:$SERVICEMIX_HOME/bin
#
echo "Servicemix account .. !! "
echo "SERVICEMIX_HOME   is : " $SERVICEMIX_HOME
cd $SERVICEMIX_HOME
#
alias adm-svmx='ssh -p 8101 admin@localhost' 
#
