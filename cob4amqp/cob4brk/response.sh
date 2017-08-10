#!/bin/bash
#
# Program chaining : 
#
# response (main) --> driver.o (driver.cob) --> res_trx.so (res_trx.cob)
#
#
cat /dev/null > response_1.log
#
./response db_connection.xml broker_connection.xml response_1.xml 1>>response_1.$PPID.log 2>>response_1.$PPID.log
#
#
