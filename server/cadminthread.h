#ifndef CADMINTHREAD_H
#define CADMINTHREAD_H

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

#include "cmysqlsession.h"
#include "cflight.h"

void* adminThreadFunc(void* param);

#endif