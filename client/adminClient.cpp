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

#define MAXBUFSIZE 10240
int startAction(int connfd);
int queryInformation(int connfd);
int deleleInformation(int connfd);

int queryFlightInformation(int connfd);
int queryReservationInformation(int connfd);
int queryFlightInformationbyDate(int connfd);
int queryAllReservation(int connfd);
int queryReservationsByName(int connfd);
int queryAllFlights(int connfd);

int deleteFlightInformation(int connfd);
int deleteReservationInformation(int connfd);
int deleteFlightInformationbyDate(int connfd);
int deleteFlightById(int connfd);

int login(int connfd);

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

	if (login(clientfd) == 0)
	{
		startAction(clientfd);
	}
	
	close(clientfd);
	return 0;
}

int login(int connfd)
{
	char buf[MAXBUFSIZE] = {0};

    while(1)
    {
    	bzero(buf, MAXBUFSIZE);
        if(recv(connfd, buf, MAXBUFSIZE, 0) < 0)
        {
            perror("read data from client");
            return -1;
        }

        printf("%s", buf);
        bzero(buf, MAXBUFSIZE);
        scanf("%s", buf);
        getchar();

        if (send(connfd, buf, strlen(buf), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }

        bzero(buf, MAXBUFSIZE);
        if(recv(connfd, buf, MAXBUFSIZE, 0) < 0)
        {
            perror("read data from client");
            return -1;
        }

        printf("%s", buf);
        bzero(buf, MAXBUFSIZE);
        scanf("%s", buf);
        getchar();

        if (send(connfd, buf, strlen(buf), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }

        bzero(buf, MAXBUFSIZE);
        if(recv(connfd, buf, 1, 0) < 0)
        {
            perror("read data from client");
            return -1;
        }

        int ret = atoi(buf);
        if (ret == 0)
        {
        	break;
        }
        else
        {
        	printf("Username or password is invalid! Please input again.\n");
        }
    }

    return 0;
}

int startAction(int connfd)
{
	char buf[MAXBUFSIZE] = {0};

	while (1)
	{
		bzero(buf, MAXBUFSIZE);
		int recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
		if (recvBytes < 0)
		{
			perror("recv Data from server");
			return -1;
		}

		printf("%s", buf);
		bzero(buf, MAXBUFSIZE);

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

		bzero(buf, MAXBUFSIZE);
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
				if (queryInformation(connfd) < 0)
				{
					return -1;
				}
			}
			else if (select == 2)
			{
				if (deleleInformation(connfd) < 0)
				{
					return -1;
				}
			}
			else
			{
				break;
			}
		}
	}

	return 0;
}

int queryInformation(int connfd)
{
	char buf[MAXBUFSIZE] = {0};
	while (1)
	{
		bzero(buf, MAXBUFSIZE);
		int recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
		if (recvBytes < 0)
		{
			perror("recv Data from server");
			return -1;
		}

		printf("%s", buf);
		bzero(buf, MAXBUFSIZE);

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

		bzero(buf, MAXBUFSIZE);
		recvBytes = recv(connfd, buf, 1, 0);
		if (recvBytes < 0)
		{
			perror("recv Data from server");
			return -1;
		}

		//std::cout << buf << std::endl;
		int ret = atoi(buf);

		if (ret == 0)
		{
			//std::cout << select << std::endl;
			if (select == 1)
			{
				if (queryFlightInformation(connfd) < 0)
				{
					return -1;
				}
			}
			else if (select == 2)
			{
				if (queryReservationInformation(connfd) < 0)
				{
					return -1;
				}
			}
			else
			{
				break;
			}
		}

	}
	return 0;
}

int queryFlightInformation(int connfd)
{
	char buf[MAXBUFSIZE] = {0};
	while (1)
	{
		bzero(buf, MAXBUFSIZE);
		int recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
		if (recvBytes < 0)
		{
			perror("recv Data from server");
			return -1;
		}

		printf("%s", buf);
		bzero(buf, MAXBUFSIZE);

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

		bzero(buf, MAXBUFSIZE);
		recvBytes = recv(connfd, buf, 1, 0);
		if (recvBytes < 0)
		{
			perror("recv Data from server");
			return -1;
		}

		//std::cout << buf << std::endl;
		int ret = atoi(buf);

		if (ret == 0)
		{
			//std::cout << select << std::endl;
			if (select == 1)
			{
				if (queryAllFlights(connfd) < 0)
				{
					return -1;
				}
			}
			else if (select == 2)
			{
				if (queryFlightInformationbyDate(connfd) < 0)
				{
					return -1;
				}
			}
			else
			{
				break;
			}
		}

	}
	return 0;
}

int queryAllFlights(int connfd)
{
	char buf[MAXBUFSIZE] = {0};

	if (recv(connfd, buf, MAXBUFSIZE, 0) < 0)
	{
		perror("recv Data from server");
		return -1;
	}
	printf("%s", buf);

	bzero(buf, MAXBUFSIZE);
	sprintf(buf, "0");
	if (send(connfd, buf, 1, 0) < 0)
	{
		return -1;
	}

	return 0;
}

int queryFlightInformationbyDate(int connfd)
{
	char buf[MAXBUFSIZE] = {0};

	int recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
	if (recvBytes < 0)
	{
		perror("recv Data from server");
		return -1;
	}

	printf("%s", buf);
	bzero(buf, MAXBUFSIZE);
	scanf("%s", buf);
	getchar();

	int sendBytes = send(connfd, buf, strlen(buf), 0);
	if (sendBytes < 0)
	{
		perror("Sending filename to server failed\n");  //fail to send the filename
		return -1;
	}

	bzero(buf, MAXBUFSIZE);
	recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
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

int queryReservationInformation(int connfd)
{
	char buf[MAXBUFSIZE] = {0};
	while (1)
	{
		bzero(buf, MAXBUFSIZE);
		int recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
		if (recvBytes < 0)
		{
			perror("recv Data from server");
			return -1;
		}

		printf("%s", buf);
		bzero(buf, MAXBUFSIZE);

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

		bzero(buf, MAXBUFSIZE);
		recvBytes = recv(connfd, buf, 1, 0);
		if (recvBytes < 0)
		{
			perror("recv Data from server");
			return -1;
		}

		//std::cout << buf << std::endl;
		int ret = atoi(buf);

		if (ret == 0)
		{
			//std::cout << select << std::endl;
			if (select == 1)
			{
				if (queryAllReservation(connfd) < 0)
				{
					return -1;
				}
			}
			else if (select == 2)
			{
				if (queryReservationsByName(connfd) < 0)
				{
					return -1;
				}
			}
			else
			{
				break;
			}
		}

	}
	return 0;
}

int queryAllReservation(int connfd)
{
	char buf[MAXBUFSIZE] = {0};

	int recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
	if (recvBytes < 0)
	{
		perror("recv Data from server");
		return -1;
	}

	int ret = atoi(buf);
	if (ret != 0)
	{
		printf("Not Found records matching the conditions\n");
		return -1;
	}
	printf("%s", buf);

	int select = 0;
	scanf("%d", &select);
	getchar();

	bzero(buf, MAXBUFSIZE);
	sprintf(buf, "%d", select);

	int sendBytes = send(connfd, buf, strlen(buf), 0);
	if (sendBytes < 0)
	{
		perror("Sending filename to server failed\n");  //fail to send the filename
		return -1;
	}

	bzero(buf, MAXBUFSIZE);
	recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
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

int queryReservationsByName(int connfd)
{
	char buf[MAXBUFSIZE] = {0};
	int recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
	if (recvBytes < 0)
	{
		perror("recv Data from server");
		return -1;
	}

	printf("%s", buf);

	char name[256] = {0};
	char c;
	int i = 0;
	while ((c = getchar()) != '\n')
	{
		name[i++] = c;
	}

	bzero(buf, MAXBUFSIZE);
	strcpy(buf, name);
	int sendBytes = send(connfd, buf, strlen(buf), 0);
	if (sendBytes < 0)
	{
		perror("Sending filename to server failed\n");  //fail to send the filename
		return -1;
	}

	bzero(buf, MAXBUFSIZE);
	recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
	if (recvBytes < 0)
	{
		perror("recv Data from server");
		return -1;
	}

	int ret = atoi(buf);
	if (ret == 1)
	{
		printf("Not found flight recorders\n");
		return 0;
	}
	printf("%s", buf);

	buf[0] = '0';
	if (send(connfd, buf, 1, 0) < 0)
	{
		return -1;
	}
	return 0;
}

int deleleInformation(int connfd)
{
	char buf[MAXBUFSIZE] = {0};
	while (1)
	{
		bzero(buf, MAXBUFSIZE);
		int recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
		if (recvBytes < 0)
		{
			perror("recv Data from server");
			return -1;
		}

		printf("%s", buf);
		bzero(buf, MAXBUFSIZE);

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

		bzero(buf, MAXBUFSIZE);
		recvBytes = recv(connfd, buf, 1, 0);
		if (recvBytes < 0)
		{
			perror("recv Data from server");
			return -1;
		}

		//std::cout << buf << std::endl;
		int ret = atoi(buf);

		if (ret == 0)
		{
			//std::cout << select << std::endl;
			if (select == 1)
			{
				if (deleteFlightInformation(connfd) < 0)
				{
					return -1;
				}
			}
			else if (select == 2)
			{
				if (deleteReservationInformation(connfd) < 0)
				{
					return -1;
				}
			}
			else
			{
				break;
			}
		}

	}
	return 0;
}

int deleteFlightInformation(int connfd)
{
	char buf[MAXBUFSIZE] = {0};
	while (1)
	{
		bzero(buf, MAXBUFSIZE);
		int recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
		if (recvBytes < 0)
		{
			perror("recv Data from server");
			return -1;
		}

		printf("%s", buf);
		bzero(buf, MAXBUFSIZE);

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

		bzero(buf, MAXBUFSIZE);
		recvBytes = recv(connfd, buf, 1, 0);
		if (recvBytes < 0)
		{
			perror("recv Data from server");
			return -1;
		}

		//std::cout << buf << std::endl;
		int ret = atoi(buf);

		if (ret == 0)
		{
			//std::cout << select << std::endl;
			if (select == 1)
			{
				if (deleteFlightById(connfd) < 0)
				{
					return -1;
				}
			}
			else if (select == 2)
			{
				if (deleteFlightInformationbyDate(connfd) < 0)
				{
					return -1;
				}
			}
			else
			{
				break;
			}
		}

	}
	return 0;
}
int deleteFlightById(int connfd)
{
	char buf[MAXBUFSIZE] = {0};

	int recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
	if (recvBytes < 0)
	{
		perror("recv Data from server");
		return -1;
	}

	int ret = atoi(buf);
	if (ret == 1)
	{
		printf("Not found flight recorders\n");
		return 0;
	}

	printf("%s", buf);
	bzero(buf, MAXBUFSIZE);

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

	bzero(buf, MAXBUFSIZE);
	recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
	if (recvBytes < 0)
	{
		perror("recv Data from server");
		return -1;
	}
	ret = atoi(buf);
	if (ret != 0)
	{
		printf("The input row number is invalid.\n");
		return 0;
	}
	printf("%s", buf);
	bzero(buf, MAXBUFSIZE);
	scanf("%s", buf);
	getchar();

	if (send(connfd, buf, strlen(buf), 0) < 0)
	{
		perror("Sending filename to server failed\n");  //fail to send the filename
		return -1;
	}

	bzero(buf, MAXBUFSIZE);
	recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
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

int deleteFlightInformationbyDate(int connfd)
{
	char buf[MAXBUFSIZE] = {0};

	int recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
	if (recvBytes < 0)
	{
		perror("recv Data from server");
		return -1;
	}

	printf("%s", buf);
	bzero(buf, MAXBUFSIZE);
	scanf("%s", buf);
	getchar();

	int sendBytes = send(connfd, buf, strlen(buf), 0);
	if (sendBytes < 0)
	{
		perror("Sending filename to server failed\n");  //fail to send the filename
		return -1;
	}

	bzero(buf, MAXBUFSIZE);
	recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
	if (recvBytes < 0)
	{
		perror("recv Data from server");
		return -1;
	}

	int ret = atoi(buf);
	if (ret == 1)
	{
		printf("Not found flight recorders\n");
		return 0;
	}
	printf("%s", buf);

	int select = 0;
	scanf("%d", &select);
	getchar();
	bzero(buf, MAXBUFSIZE);
	sprintf(buf, "%d", select);

	sendBytes = send(connfd, buf, strlen(buf), 0);
	if (sendBytes < 0)
	{
		perror("Sending filename to server failed\n");  //fail to send the filename
		return -1;
	}

	bzero(buf, MAXBUFSIZE);
	recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
	if (recvBytes < 0)
	{
		perror("recv Data from server");
		return -1;
	}
	ret = atoi(buf);
	if (ret != 0)
	{
		printf("The input row number is invalid.\n");
		return 0;
	}

	printf("%s", buf);
	bzero(buf, MAXBUFSIZE);
	scanf("%s", buf);
	getchar();

	if (send(connfd, buf, strlen(buf), 0) < 0)
	{
		perror("Sending filename to server failed\n");  //fail to send the filename
		return -1;
	}

	bzero(buf, MAXBUFSIZE);
	recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
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

int deleteReservationInformation(int connfd)
{
	char buf[MAXBUFSIZE] = {0};
	int recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
	if (recvBytes < 0)
	{
		perror("recv Data from server");
		return -1;
	}

	printf("%s", buf);

	char name[256] = {0};
	char c;
	int i = 0;
	while ((c = getchar()) != '\n')
	{
		name[i++] = c;
	}

	bzero(buf, MAXBUFSIZE);
	strcpy(buf, name);
	int sendBytes = send(connfd, buf, strlen(buf), 0);
	if (sendBytes < 0)
	{
		perror("Sending filename to server failed\n");  //fail to send the filename
		return -1;
	}

	bzero(buf, MAXBUFSIZE);
	recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
	if (recvBytes < 0)
	{
		perror("recv Data from server");
		return -1;
	}

	int ret = atoi(buf);
	if (ret == 1)
	{
		printf("Not found flight recorders\n");
		return 0;
	}
	printf("%s", buf);

	int select = 0;
	scanf("%d", &select);
	getchar();
	bzero(buf, MAXBUFSIZE);
	sprintf(buf, "%d", select);

	sendBytes = send(connfd, buf, strlen(buf), 0);
	if (sendBytes < 0)
	{
		perror("Sending filename to server failed\n");  //fail to send the filename
		return -1;
	}

	bzero(buf, MAXBUFSIZE);
	recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
	if (recvBytes < 0)
	{
		perror("recv Data from server");
		return -1;
	}
	ret = atoi(buf);
	if (ret != 0)
	{
		printf("The input row number is invalid.\n");
		return 0;
	}
	
	printf("%s", buf);
	bzero(buf, MAXBUFSIZE);
	scanf("%s", buf);
	getchar();

	if (send(connfd, buf, strlen(buf), 0) < 0)
	{
		perror("Sending filename to server failed\n");  //fail to send the filename
		return -1;
	}

	bzero(buf, MAXBUFSIZE);
	recvBytes = recv(connfd, buf, MAXBUFSIZE, 0);
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
