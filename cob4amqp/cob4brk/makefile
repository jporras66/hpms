#
include ../makefile.inc
#
all: request response cobclient.so req_trx.so res_trx.so
#
clean:
	rm -f request response *.lst *.c.h *.c.l.h *.i
	rm -f *.o *.so *.dll 
	rm -f driver.c driver.cob cobclient.c
	rm -f req_trx.c req_trx.cob res_trx.c res_trx.cob 
#
driver.o:  driver.pco
	ocesql driver.pco driver.cob
	$(COBC) $(COBFLAGS) -c driver.cob  -t driver.lst
#rm -f  driver.c 	driver.c.h 		driver.c.l.h 	driver.lst
#
#timer.o:  timer.pco
#	ocesql timer.pco timer.cob
#	$(COBC) $(COBFLAGS) -c timer.cob  -t timer.lst
#
#queueTimeout.o: queueTimeout.cpp
#	$(CXX) $(GCCFLASG_11)  queueTimeout.cpp  $(INCLUDE)
# 
# please link $(AMQPLIB) before $(COBLIB) :
# src/qpid/sys/posix/Mutex.h:116: void qpid::sys::Mutex::lock(): La declaración `0' no se cumple.
#
request: queueListen.cpp driver.o
	$(CXX) $(GCCFLASG)  queueListen.cpp driver.o -o request  $(INCLUDE) $(APLLIB) $(AMQPLIB) $(COBLIB) $(DBLIB)
#
#timeout: queueTimeout.cpp timer.o ../common/sleep.o
#	$(CXX) $(GCCFLASG_C11) queueTimeout.cpp ../common/sleep.o timer.o -o timeout  $(INCLUDE) $(APLLIB) $(AMQPLIB) $(COBLIB) $(DBLIB) -lpthread
#
response: queueListen.cpp driver.o
	$(CXX) $(GCCFLASG) queueListen.cpp driver.o -o response  $(INCLUDE) $(APLLIB) $(AMQPLIB) $(COBLIB) $(DBLIB)
#
cobclient.so:  cobclient.cob ../lib/libamqpbrk.so$(SUFX)
	$(COBC) $(COBFLAGS) -m cobclient.cob  -t cobclient.lst
#
req_trx.so: req_trx.pco ../lib/libamqpbrk.so$(SUFX)
	ocesql req_trx.pco req_trx.cob
	$(COBC) $(COBFLAGS)  -m req_trx.cob  -o req_trx.so -t req_trx.lst
res_trx.so: res_trx.pco ../lib/libamqpbrk.so$(SUFX)
	ocesql res_trx.pco res_trx.cob
	$(COBC) $(COBFLAGS)  -m res_trx.cob  -o res_trx.so -t res_trx.lst
#
.SUFFIXES: .pco
#
#send_m.so:  send_m.cob ../lib/libamqpbrk.so$(SUFX)
#$(COBC) $(COBFLAGS) -m send_m.cob  -t send_m.lst
#rm -f  send_m.c 	send_m.c.h 	send_m.c.l.h 	send_m.lst
#
