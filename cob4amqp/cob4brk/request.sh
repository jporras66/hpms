#!/bin/bash
#
# Program chaining : 
#
# request (main) --> driver.o (driver.cob) --> req_trx.so (req_trx.cob)
#
cat /dev/null > request_1.log 
#
./request db_connection.xml broker_connection.xml request_1.xml 1>>request_1.$PPID.log 2>>request_1.$PPID.log 
#
#
