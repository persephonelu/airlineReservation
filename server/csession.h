#ifndef CSESSION_H
#define CSESSION_H

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
#include <iostream>

class CSession
{
public:
	CSession()
	{
		FD_ZERO(&readfds);
		m_adminfd 	= -1;
		m_userfd 	= -1;
		m_airlinefd	= -1;
	}
	~CSession()
	{
		close(m_adminfd);
		close(m_userfd);
		close(m_airlinefd);
	}

	/* data */
	int initializeSocket(const char* , const int arilinePort, const int userPort, const int adminPort);
	int initializeAdminServer(const char*, int adminPort);
	int initializeUserServer(const char*, int userPort);
	int initializeAirlineServer(const char*, int airlinePort);
	int startSevice();

private:
	int m_adminfd;
	int m_userfd;
	int m_airlinefd;

	fd_set readfds;
};

#endif
