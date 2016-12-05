#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <ctype.h>
#include <limits.h>
#include <iostream>

#define MAXBUFFSIZE 10240

char buf[MAXBUFFSIZE];

int logon(int connfd);
int signup(int connfd);
int signin(int connfd);
int startService(int connfd);
int addFlight(int connfd);
int listFlight(int connfd);
int delFlight(int connfd);

int main(int argc, char** argv)
{
	int clientfd;
	struct sockaddr_in dest_addr;
	char filename[100] = {0};

	//if do not have serverIP, server port and number, exit.
	if (argc != 3)
	{
		printf("usage: ./tcpSquareClient serverip serverport.\n eg: ./tcpSquareClient 127.0.0.1 2000\n");
		exit(1);
	}

	//Create a socket
	clientfd = socket(PF_INET, SOCK_STREAM, 0);
	if (clientfd < 0)
	{
		perror("Create client socket failed\n");
		exit(1);
	}

	//initiate struct dest_addr with 0
	bzero(&dest_addr, sizeof(struct sockaddr_in));
	//initiate destination IP address and port.
	int destport = atoi(argv[2]);
	dest_addr.sin_family = AF_INET;			//TCP/IP protocol
	dest_addr.sin_port = htons(destport);	//server port
	dest_addr.sin_addr.s_addr = inet_addr(argv[1]);	// server IP address

	//connect server
	if (connect(clientfd, (struct sockaddr*)&dest_addr, sizeof(struct sockaddr)) < 0)
	{
		perror("Connect server Failed\n");
		exit(1);
	}

    if (logon(clientfd) == 0)
    {
        startService(clientfd);
    }
	
	close(clientfd);

	return 0;
}

int logon(int connfd)
{
    while (1)
    {
        bzero(buf, MAXBUFFSIZE);
        if (recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
        {
            perror("recv Data from server");
            return -1;
        }

        printf("%s", buf);
        bzero(buf, MAXBUFFSIZE);

        int select = 0;
        scanf("%d", &select);
        getchar();
        sprintf(buf, "%d", select);
        if (send(connfd, buf, strlen(buf), 0) < 0)
        {
            perror("Sending filename to server failed\n");  //fail to send the filename
            return -1;
        }

        bzero(buf, MAXBUFFSIZE);
        if (recv(connfd, buf, 1, 0) < 0)
        {
            perror("recv Data from server");
            return -1;
        }

        int ret = atoi(buf);

        if (ret == 0)
        {
            if (select == 1)
            {
                if (signup(connfd) == 0)
                {
                    return 0;
                }
            }
            else if (select == 2)
            {
                if (signin(connfd) == 0)
                {
                    return 0;
                }
            }
            else if (select == 3)
            {
                return 1;
            }
        }
    }
    return 0;
}

int signup(int connfd)
{
    //username
    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }
    printf("%s", buf);

    bzero(buf, MAXBUFFSIZE);
    scanf("%[^\n]", buf);
    getchar();
    if (send(connfd, buf, strlen(buf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }

    //password
    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }
    printf("%s", buf);

    bzero(buf, MAXBUFFSIZE);
    scanf("%[^\n]", buf);
    getchar();
    if (send(connfd, buf, strlen(buf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }

    //result
    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, 1, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }
    int ret = atoi(buf);
    if (ret == 1)
    {
        printf("Sign up failed. Please retry.\n");
        return -1;
    }

    printf("Sign up success.\n");
    return 0;
}

int signin(int connfd)
{
    //username
    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }
    printf("%s", buf);

    bzero(buf, MAXBUFFSIZE);
    scanf("%[^\n]", buf);
    getchar();
    if (send(connfd, buf, strlen(buf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }

    //password
    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }
    printf("%s", buf);

    bzero(buf, MAXBUFFSIZE);
    scanf("%[^\n]", buf);
    getchar();
    if (send(connfd, buf, strlen(buf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }

    //result
    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, 1, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }
    int ret = atoi(buf);
    if (ret == 1)
    {
        printf("Sign in Failed. Please retry.\n");
        return -1;
    }

    printf("Sign in success.\n");
    return 0;
}

int startService(int connfd)
{
	while (1)
	{
		bzero(buf, MAXBUFFSIZE);
		int recvBytes = recv(connfd, buf, MAXBUFFSIZE, 0);
		if (recvBytes < 0)
		{
			perror("recv Data from server");
			return -1;
		}

		printf("%s", buf);
		bzero(buf, MAXBUFFSIZE);

		int select = 0;
		scanf("%d", &select);
		getchar();

		sprintf(buf, "%d", select);
		int sendBytes = send(connfd, buf, strlen(buf), 0);
		if (sendBytes < 0)
		{
			perror("Sending filename to server failed\n");  //fail to send the filename
			return -1;
		}

		bzero(buf, MAXBUFFSIZE);
		recvBytes = recv(connfd, buf, 1, 0);
		if (recvBytes < 0)
		{
			perror("recv Data from server");
			return -1;
		}

		int ret = atoi(buf);

		if (ret == 0)
		{
			if (select == 1)
			{
				if (listFlight(connfd) < 0)
				{
					return -1;
				}
			}
			else if (select == 2)
			{
				if (addFlight(connfd) < 0)
                {
                    return -1;
                }
			}
            else if (select == 3)
            {
                if (delFlight(connfd) < 0)
                {
                    return -1;
                }
            }
            else if (select == 4)
            {
                break;
            }
		}
	}

	return 0;
}

int listFlight(int connfd)
{
    bzero(buf, MAXBUFFSIZE);

    if (recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("recv Data from server");
        return -1;
    }
    printf("%s", buf);

    bzero(buf, MAXBUFFSIZE);
    sprintf(buf, "0");
    if (send(connfd, buf, 1, 0) < 0)
    {
        return -1;
    }
    return 0;
}

int addFlight(int connfd)
{
	//flight number
    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }
    printf("%s", buf);

    bzero(buf, MAXBUFFSIZE);
    scanf("%[^\n]", buf);
    getchar();
    if (send(connfd, buf, strlen(buf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }
    int select = atoi(buf);
    if (select == -1)
    {
    	printf("Add flight Aborted!\n");
    	return 0;
    }

    //depart day
    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }
    printf("%s", buf);

    bzero(buf, MAXBUFFSIZE);
    scanf("%[^\n]", buf);
    getchar();
    if (send(connfd, buf, strlen(buf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }
    select = atoi(buf);
    if (select == -1)
    {
    	printf("Add flight Aborted!\n");
    	return 0;
    }

    //departhour
    while (1)
    {
    	bzero(buf, MAXBUFFSIZE);
    	if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    	{
        	perror("read data from client");
        	return -1;
    	}
    	printf("%s", buf);

    	bzero(buf, MAXBUFFSIZE);
    	scanf("%[^\n]", buf);
    	getchar();
    	if (send(connfd, buf, strlen(buf), 0) < 0)
    	{
        	perror("Send data to client");
        	return -1;
    	}
    	select = atoi(buf);
    	if (select == -1)
    	{
    		printf("Add flight Aborted!\n");
    		return 0;
    	}

    	if(recv(connfd, buf, 1, 0) < 0)
    	{
        	perror("read data from client");
        	return -1;
    	}

    	if (buf[0] == '0')
    	{
    		break;
    	}
    	else
    	{
    		printf("Please inpurt a valid hour (from 0 to 23).\n");
    	}
    }   

    //depart min
    while (1)
    {
    	bzero(buf, MAXBUFFSIZE);
    	if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    	{
        	perror("read data from client");
        	return -1;
    	}
   		printf("%s", buf);

    	bzero(buf, MAXBUFFSIZE);
    	scanf("%[^\n]", buf);
    	getchar();
    	if (send(connfd, buf, strlen(buf), 0) < 0)
    	{
        	perror("Send data to client");
        	return -1;
    	}
    	select = atoi(buf);
    	if (select == -1)
    	{
    		printf("Add flight Aborted!\n");
    		return 0;
    	}

    	if(recv(connfd, buf, 1, 0) < 0)
    	{
        	perror("read data from client");
        	return -1;
    	}

    	if (buf[0] == '0')
    	{
    		break;
    	}
    	else
    	{
    		printf("Please inpurt a valid Minute (from 0 to 59).\n");
    	}
    }
    
    //source
    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }
    printf("%s", buf);

    bzero(buf, MAXBUFFSIZE);
    scanf("%[^\n]", buf);
    getchar();
    if (send(connfd, buf, strlen(buf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }
    select = atoi(buf);
    if (select == -1)
    {
    	printf("Add flight Aborted!\n");
    	return 0;
    }

    //destination
    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }
    printf("%s", buf);

    bzero(buf, MAXBUFFSIZE);
    scanf("%[^\n]", buf);
    getchar();
    if (send(connfd, buf, strlen(buf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }
    select = atoi(buf);
    if (select == -1)
    {
    	printf("Add flight Aborted!\n");
    	return 0;
    }

    //seat
    while (1)
    {
    	bzero(buf, MAXBUFFSIZE);
    	if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    	{
        	perror("read data from client");
        	return -1;
    	}
   		printf("%s", buf);

    	bzero(buf, MAXBUFFSIZE);
    	scanf("%[^\n]", buf);
    	getchar();
    	if (send(connfd, buf, strlen(buf), 0) < 0)
    	{
        	perror("Send data to client");
        	return -1;
    	}
    	select = atoi(buf);
    	if (select == -1)
    	{
    		printf("Add flight Aborted!\n");
    		return 0;
    	}

    	if(recv(connfd, buf, 1, 0) < 0)
    	{
        	perror("read data from client");
        	return -1;
    	}

    	if (buf[0] == '0')
    	{
    		break;
    	}
    	else
    	{
    		printf("Please the seat number must be a valid number (great than 0).\n");
    	}
    }

    //result
    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }
    printf("%s", buf);

    buf[0] = '0';
    if (send(connfd, buf, 1, 0) < 0)
    {
    	return -1;
    }

    return 0;
}

int delFlight(int connfd)
{
    bzero(buf, MAXBUFFSIZE);

    int recvBytes = recv(connfd, buf, MAXBUFFSIZE, 0);
    if (recvBytes < 0)
    {
        perror("recv Data from server");
        return -1;
    }

    int ret = atoi(buf);
    if (ret == 1)
    {
        printf("Not found flight recorders\n");
        buf[0] = '1';
        if (send(connfd, buf, 1, 0) < 0)
        {
            return -1;
        }
        return 0;
    }

    printf("%s", buf);
    bzero(buf, MAXBUFFSIZE);

    int select = 0;
    scanf("%d", &select);
    getchar();
    sprintf(buf, "%d", select);

    int sendBytes = send(connfd, buf, strlen(buf), 0);
    if (sendBytes < 0)
    {
        perror("Sending filename to server failed\n");  //fail to send the filename
        return -1;
    }

    bzero(buf, MAXBUFFSIZE);
    recvBytes = recv(connfd, buf, MAXBUFFSIZE, 0);
    if (recvBytes < 0)
    {
        perror("recv Data from server");
        return -1;
    }

    ret = atoi(buf);
    if (ret != 0)
    {
        printf("The input row number is invalid.\n");
        buf[0] = '1';
        if (send(connfd, buf, 1, 0) < 0)
        {
            return -1;
        }
        return 0;
    }
    printf("%s", buf);
    bzero(buf, MAXBUFFSIZE);
    scanf("%s", buf);
    getchar();

    if (send(connfd, buf, strlen(buf), 0) < 0)
    {
        perror("Sending filename to server failed\n");  //fail to send the filename
        return -1;
    }

    bzero(buf, MAXBUFFSIZE);
    recvBytes = recv(connfd, buf, MAXBUFFSIZE, 0);
    if (recvBytes < 0)
    {
        perror("recv Data from server");
        return -1;
    }
    printf("%s", buf);

    buf[0] = '0';
    if (send(connfd, buf, 1, 0) < 0)
    {
        return -1;
    }

    return 0;
}