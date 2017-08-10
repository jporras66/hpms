#
include ../makefile.inc  
#
all: amqpbrk.o ../lib/libamqpbrk.so$(SUFX) consumer producer browser
#
clean:
	rm -f *.o *.i producer consumer browser ../lib/libamqpbrk.so$(SUFX)
#
amqpbrk.o: amqpbrk.cpp amqpbrk.h message.h
	$(CXX) $(GCCFLASG) $(FPIC) -c amqpbrk.cpp $(INCLUDE)
#
amqpbrk_c.o: amqpbrk_c.c amqpbrk.h message.h
	$(CXX) $(GCCFLASG) $(FPIC) -c amqpbrk_c.c $(INCLUDE)
#		
../lib/libamqpbrk.so$(SUFX): amqpbrk.o amqpbrk_c.o ../common/common.o ../common/xmlcfg.o ../common/asc2ebc.o ../common/sleep.o ../common/cbl_oc_dump.o
	$(CXX) -shared -Wl,-soname,libamqpbrk.so amqpbrk.o amqpbrk_c.o \
	../common/common.o ../common/xmlcfg.o ../common/asc2ebc.o ../common/sleep.o ../common/cbl_oc_dump.o   \
	-o ../lib/libamqpbrk.so$(SUFX)
#	
consumer: consumer.cpp message.h amqpbrk.cpp ../common/xmlcfg.cpp ../common/common.c
	$(CXX) $(GCCFLASG) $(FPIC) consumer.cpp amqpbrk.o ../common/xmlcfg.o ../common/common.o -o consumer $(INCLUDE) $(AMQPLIB)
#	
producer: producer.cpp message.h amqpbrk.cpp ../common/xmlcfg.cpp ../common/common.c
	$(CXX) $(GCCFLASG) $(FPIC) producer.cpp amqpbrk.o ../common/xmlcfg.o ../common/common.o -o producer $(INCLUDE) $(AMQPLIB) 
#
browser: browser.cpp message.h  amqpbrk.cpp ../common/xmlcfg.cpp ../common/common.c
	$(CXX) $(GCCFLASG) $(FPIC) browser.cpp  amqpbrk.o ../common/xmlcfg.o ../common/common.o -o browser  $(INCLUDE) $(AMQPLIB) 
#
