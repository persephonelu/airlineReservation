#ifndef CTHREAD_H
#define CTHREAD_H

#include <string>
#include <vector>
#include <iostream>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <unistd.h>
#include <fcntl.h>
#include <arpa/inet.h>
#include <pthread.h>

class CThread {
public:
	CThread(){}
	virtual ~CThread(){}
	virtual void* run() = 0;
	void start();
private:
	pthread_t tid;
	static void* threadFunc(void* param);
};

#endif
