CC=gcc
CPP=g++
CFLAGS= -o
INCLUDE=-I/usr/local/include
LIB=/usr/local/lib/libmysqlclient.a
OBJ= main.o csession.o cadminthread.o cuserthread.o cairlinethread.o cmysqlsession.o
LDFLAGS=-lpthread

all: airlineReservationServer

airlineReservationServer:	${OBJ}
	${CPP} -g -o airlineReservationServer ${OBJ} ${INCLUDE} ${LIB} ${LDFLAGS}

main.o:	main.cpp
	${CPP} -c main.cpp ${INCLUDE}

csession.o:	csession.cpp
	${CPP} -c csession.cpp ${INCLUDE}

cadminthread.o:	cadminthread.cpp
	${CPP} -c cadminthread.cpp ${INCLUDE}

cuserthread.o:	cuserthread.cpp
	${CPP} -c cuserthread.cpp ${INCLUDE}

cairlinethread.o:	cairlinethread.cpp
	${CPP} -c cairlinethread.cpp ${INCLUDE}

cmysqlsession.o:	cmysqlsession.cpp
	${CPP} -c cmysqlsession.cpp ${INCLUDE}


clean:
	rm *.o airlineReservationServer
