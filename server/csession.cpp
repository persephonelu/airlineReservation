#include "csession.h"
#include <pthread.h>
#include "cuserthread.h"
#include "cadminthread.h"
#include "cairlinethread.h"

int CSession::initializeAdminServer(const char* serverIP, int adminPort)
{
	m_adminfd = socket(PF_INET, SOCK_STREAM, 0);
	if (m_adminfd < 0)
	{
		return -1;
	}

	struct sockaddr_in server_addr;

    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(adminPort);
    server_addr.sin_addr.s_addr = inet_addr(serverIP);

    if(bind(m_adminfd,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0)
    {
        return -1;
    }

    return 0;
}

int CSession::initializeUserServer(const char* serverIP, int userPort)
{
	m_userfd = socket(PF_INET, SOCK_STREAM, 0);;
	if (m_userfd < 0)
	{
		return -1;
	}
	
	struct sockaddr_in server_addr;

    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(userPort);
    server_addr.sin_addr.s_addr = inet_addr(serverIP);

    if(bind(m_userfd,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0)
    {
        return -1;
    }

    return 0;
}

int CSession::initializeAirlineServer(const char* serverIP, int airlinePort)
{
	m_airlinefd = socket(PF_INET, SOCK_STREAM, 0);
	if (m_airlinefd < 0)
	{
		return -1;
	}
	
	struct sockaddr_in server_addr;

    bzero(&server_addr,sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(airlinePort);
    server_addr.sin_addr.s_addr = inet_addr(serverIP);

    if(bind(m_airlinefd,(struct sockaddr*)&server_addr,sizeof(server_addr)) < 0)
    {
        return -1;
    }

    return 0;
}

int CSession::initializeSocket(const char* serverIP, const int airlinePort, const int userPort, const int adminPort)
{
	int ret = initializeAirlineServer(serverIP, airlinePort);
	ret 	= initializeAdminServer(serverIP, adminPort);
	ret 	= initializeUserServer(serverIP, userPort);

	return ret;
} 

int CSession::startSevice()
{
	FD_SET(m_airlinefd, &readfds);
	FD_SET(m_adminfd, &readfds);
	FD_SET(m_userfd, &readfds);
	
	if(listen(m_airlinefd,10) < 0)
    {
        perror("Failed to listen");
        return -1;
    } 

    if(listen(m_adminfd,10) < 0)
    {
        perror("Failed to listen");
        return -1;
    }

    if(listen(m_userfd,10) < 0)
    {
        perror("Failed to listen");
        return -1;
    }

    int nfds = std::max(m_airlinefd, std::max(m_userfd, m_adminfd));
    printf("server start...\n");
    while (1)
    {

    	int ret = select(nfds + 1, &readfds, NULL, NULL, NULL);
    	if (ret <= 0)
    	{
    		return -1;
    	}

    	if (FD_ISSET(m_airlinefd, &readfds))
    	{
    		int connfd = accept(m_airlinefd, NULL, NULL);
    		if (connfd < 0)
    		{
    			perror("accept error\n");
    			continue;
    		}

            pthread_t tid;
            pthread_create(&tid, NULL, &airlineThreadFunc, (void*)&connfd);
    	}

    	if (FD_ISSET(m_userfd, &readfds))
    	{
    		int connfd = accept(m_userfd, NULL, NULL);
    		if (connfd < 0)
    		{
    			perror("accept error\n");
    			continue;
    		}

    		pthread_t tid;
            pthread_create(&tid, NULL, &userThreadFunc, (void*)&connfd);
    	}

    	if (FD_ISSET(m_adminfd, &readfds))
    	{
    		int connfd = accept(m_adminfd, NULL, NULL);
    		if (connfd < 0)
    		{
    			perror("accept error\n");
    			continue;
    		}

    		pthread_t tid;
            pthread_create(&tid, NULL, &adminThreadFunc, (void*)&connfd);
    	}
    }

	return 0;
}