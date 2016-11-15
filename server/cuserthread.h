#ifndef CUSERTHREAD_H
#define CUSERTHREAD_H

#include "cmysqlsession.h"
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

void* userThreadFunc(void* param);

#endif