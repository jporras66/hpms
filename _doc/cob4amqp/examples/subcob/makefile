#
include ../include.mk 
#  
all: mainc 
#
clean:
	rm -f mainc *.o *.lst *.c.h *.c.l.h log.* subcob.c subcob.s
subcob.s:  subcob.cob
	$(COBC) $(COBFLAGS) -S subcob.cob  -t subcob.lst
mainc: subcob.s mainc.c
	$(CC) -O0 -g3 mainc.c subcob.s -o mainc $(INCLUDE) $(LIBS) 
