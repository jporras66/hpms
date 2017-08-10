#
ifeq ($(OS),Windows_NT)
	APLLIB=-L ../lib -lamqpbrk.a
	SUFX=.a
	FPIC=
	DEBUG=true
else
	APLLIB=-L $(APL)/lib -lamqpbrk
	SUFX=
	FPIC=-fPIC
endif 
#
CXX=g++
CC=gcc
INCLUDE=-I . -I ../common -I ../amqpbrk -I /usr/include -I /usr/local/include -I /opt/opencobol/include -I /usr/pgsql-9.5/include
LIB=-L /lib64
COBLIB=-L /opt/opencobol/lib/ -lcob 
AMQPLIB=-lqpidtypes -lqpidmessaging
#
# DB libs for precompiler and Postgresql 
DBLIB=-L/opt/ocesql/lib -locesql -L/usr/pgsql-9.5/lib/ -lpq
#
# -DDEBUG to display DEBUG info
#
#DEBUG=-DDEBUG
DEBUG=
#GCCFLASG=-O0 -g3 -DDEBUG
GCCFLASG=-O0 -g3 -std=c99 $(DEBUG)
GCCFLASG_C11=-O0 -g3 -std=c++11 $(DEBUG) 
#GCCFLASG=-O0 -g3 
# 
COBC=/opt/opencobol/bin/cobc
#COBFLAGS=-std=cobol85 -g
COBFLAGS=-std=default -g
#
