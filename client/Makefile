CC=gcc
CFLAGS=-c
LDFLAGS=-lpthread

all: adminClient userClient airlineClient

adminClient:	adminClient.o
	${CC} -g -o adminClient adminClient.o

userClient:	userClient.o
	${CC} -g -o userClient userClient.o

airlineClient:	airlineClient.o
	${CC} -g -o airlineClient airlineClient.o

%.o:%.c
	${CC} ${CFLAGS} $< -o $@

clean:
	rm *.o adminClient userClient airlineClient
