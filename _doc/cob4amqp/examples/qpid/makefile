#
include ../../makefile.inc  
#
all: hello_world 
#
clean:
	rm -f *.o *.i hello_world
#
hello_world: hello_world.cpp 
	$(CXX) -O0 -g3 -o hello_world hello_world.cpp $(INCLUDE) $(AMQPLIB)
#
