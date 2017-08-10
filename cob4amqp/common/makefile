#
#
include ../makefile.inc 
#
all: test2xmlcfg test2ebcdic testmicrosleep
#
clean:
	rm -f *.o *.i test2xmlcfg test2ebcdic testmicrosleep
# 
common.o: common.c common.h
	$(CC) $(GCCFLASG) $(FPIC)  -c common.c $(INCLUDE)
# 
sleep.o: sleep.c sleep.h
	$(CC) $(GCCFLASG) $(FPIC)  -c sleep.c $(INCLUDE)
#
xmlcfg.o: xmlcfg.cpp xmlcfg.h
	$(CXX) $(GCCFLASG) $(FPIC) -c xmlcfg.cpp $(INCLUDE)	
#	
test2xmlcfg: common.o  xmlcfg.o test2xmlcfg.cpp
	$(CXX) $(GCCFLASG) $(FPIC) -o test2xmlcfg test2xmlcfg.cpp xmlcfg.o common.o  $(INCLUDE) 
#	
testmicrosleep: sleep.o testmicrosleep.c
	$(CC) $(GCCFLASG) $(FPIC) -o testmicrosleep testmicrosleep.c sleep.o  $(INCLUDE) 	
#
asc2ebc.o: asc2ebc.c asc2ebc.h
	gcc $(GCCFLASG) $(FPIC)  -c asc2ebc.c $(INCLUDE)
#
cbl_oc_dump.c:  cbl_oc_dump.cob
	$(COBC) $(COBFLAGS) -C cbl_oc_dump.cob -t cbl_oc_dump.lst	
cbl_oc_dump.o:  cbl_oc_dump.c
	gcc $(GCCFLASG) $(FPIC)  -c cbl_oc_dump.c $(INCLUDE)
	rm -f cbl_oc_dump.c 	cbl_oc_dump.c.h 	cbl_oc_dump.c.l.h 	cbl_oc_dump.lst
#	
test2ebcdic:  test2ebcdic.cob asc2ebc.o cbl_oc_dump.o
	$(COBC) $(COBFLAGS) -x test2ebcdic.cob  asc2ebc.o cbl_oc_dump.o -t test2ebcdic.lst
	rm -f test2ebcdic.c 	test2ebcdic.c.h 	test2ebcdic.c.l.h 	test2ebcdic.lst
