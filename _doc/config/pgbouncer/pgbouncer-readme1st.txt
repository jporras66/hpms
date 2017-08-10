PGBouncer - PostgreSQL Connection Pooling

	- Not mandatory but desiderable for high load  
	- To install pgbouncer:
	
		yum install pgbouncer

	- Config directory for pgbouncer is : /etc/pgbouncer
		
	1 Notes :	https://opensourcedbms.com/dbms/setup-pgbouncer-connection-pooling-for-postgresql-on-centosredhatfedora/

	2 Rename pgbouncer.ini to pgbouncer.ini.orig
		 
		 Copy  pgbouncer.ini from ./_doc/config/pgbouncer to /etc/pgbouncer
		 Copy  userlist.txt  from ./_doc/config/pgbouncer to /etc/pgbouncer
		 
	3 Verify and Change pgbouncer.ini

		[databases]
		template1 = host=sagitta port=5432 dbname=template1		<-- change hostname for your postgresql hostname
		HPS-CP    = host=sagitta port=5432 dbname=HPS			<-- HPS is the PostgreSQL DB that we have created previosly
																	HPS-CP will be the connection pooling name for HPS 
		; use custom pool sizes
		nondefaultdb = pool_size=100 reserve_pool_size=10
	
		[pgbouncer]
		#logfile = /var/log/postgresql/pgbouncer.log
		logfile = /var/log/pgbouncer/pgbouncer.log
		#pidfile = /var/run/postgresql/pgbouncer.pid
		pidfile = /var/run/pgbouncer/pgbouncer.pid
	
		mkdir -p /var/log/pgbouncer
		chown -R pgbouncer:pgbouncer /var/log/pgbouncer

	4 pgbouncer service :

		systemctl enable   pgbouncer.service
		systemctl disable  pgbouncer.service
		systemctl start    pgbouncer.service
		systemctl stop     pgbouncer.service
		systemctl restart  pgbouncer.service
		systemctl status   pgbouncer.service

	5 Test Connection :
	
		[root@sagitta pgbouncer]# psql -h sagitta -p 6432 -d HPS-CP -U hps
		Password for user hps:
		psql (9.5.6)
		Type "help" for help.

		HPS-CP=# select * from messages ;
		 msg_id | msg_uuid | msg_mimetype | msg_content | msg_len
		--------+----------+--------------+-------------+---------
		(0 rows)