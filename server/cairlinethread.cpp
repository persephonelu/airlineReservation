#include "cairlinethread.h"
#include "cflight.h"

#define MAXBUFFSIZE 10240

char airbuf[MAXBUFFSIZE];
int startService(int connfd);
int addFlight(int connfd);

void* airlineThreadFunc(void* param)
{
    int connfd = *(int*)param;
    startService(connfd);

    close(connfd);
    pthread_exit(NULL);
}

int startService(int connfd)
{
    bzero(airbuf, MAXBUFFSIZE);
    sprintf(airbuf, "Welcome!\nPlease Select action:\n1) Add a flight\n" \
        "2) Exit\nPress 1 or 2:\n");

    while (1)
    { 
        if (send(connfd, airbuf, strlen(airbuf), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }

        bzero(airbuf, MAXBUFFSIZE);
        if(recv(connfd, airbuf, MAXBUFFSIZE, 0) < 0)
        {
            perror("read data from client");
            return -1;
        }

        int ret = atoi(airbuf);
        //printf("%s\n", buf);
        if (ret == 1 || ret == 2)
        {
            bzero(airbuf, MAXBUFFSIZE);
            sprintf(airbuf, "0");
            if (send(connfd, airbuf, strlen(airbuf), 0) < 0)
            {
                return -1;
            }

            if (ret == 1)
            {
                if (addFlight(connfd) < 0)
                {
                    return -1;
                }
            }
            else
            {
                break;
            }
        }
        bzero(airbuf, MAXBUFFSIZE);
        sprintf(airbuf, "Please Select action:\n1) Add a flight\n" \
        "2) Exit\nPress 1 or 2:\n");
    } 

    return 0;
}

int addFlight(int connfd)
{
    Flight flight;
    int ret = 0;
    bzero(airbuf, MAXBUFFSIZE);
    sprintf(airbuf, "Please input the flight Name (If you do not want to add a flight, input -1 to abort):");
    if (send(connfd, airbuf, strlen(airbuf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }

    bzero(airbuf, MAXBUFFSIZE);
    if(recv(connfd, airbuf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }
    ret = atoi(airbuf);
    if (ret == -1)
    {
        return 0;
    }

    flight.flightNum = airbuf;

    bzero(airbuf, MAXBUFFSIZE);
    sprintf(airbuf, "Please input the depart Day (Date format is YYYY-mm-dd. If you do not want to add a flight, input -1 to abort):");
    if (send(connfd, airbuf, strlen(airbuf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }

    bzero(airbuf, MAXBUFFSIZE);
    if(recv(connfd, airbuf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }
    ret = atoi(airbuf);
    if (ret == -1)
    {
        return 0;
    }
    flight.departDay = airbuf;

    while (1)
    {
        bzero(airbuf, MAXBUFFSIZE);
        sprintf(airbuf, "Please input depart Hour (from 0 to 23. If you do not want to add a flight, input -1 to abort):");
        if (send(connfd, airbuf, strlen(airbuf), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }

        bzero(airbuf, MAXBUFFSIZE);
        if(recv(connfd, airbuf, MAXBUFFSIZE, 0) < 0)
        {
            perror("read data from client");
            return -1;
        }
        ret = atoi(airbuf);
        if (ret == -1)
        {
            return 0;
        }
        else if (!isdigit(airbuf[0]) || ret < 0 || ret > 23)
        {
            airbuf[0] = '1';
            if (send(connfd, airbuf, 1, 0) < 0)
            {
                return -1;
            }
        }
        else
        {
            flight.departHour = airbuf;
            airbuf[0] = '0';
            if (send(connfd, airbuf, 1, 0) < 0)
            {
                return -1;
            }
            break;
        }   
    }
    
    while (1)
    {
        bzero(airbuf, MAXBUFFSIZE);
        sprintf(airbuf, "Please input depart Minute (from 0 to 59. If you do not want to add a flight, input -1 to abort):");
        if (send(connfd, airbuf, strlen(airbuf), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }

        bzero(airbuf, MAXBUFFSIZE);
        if(recv(connfd, airbuf, MAXBUFFSIZE, 0) < 0)
        {
            perror("read data from client");
            return -1;
        }
        ret = atoi(airbuf);
        if (ret == -1)
        {
            return 0;
        }
        else if (!isdigit(airbuf[0]) || ret < 0 || ret > 59)
        {
            airbuf[0] = '1';
            if (send(connfd, airbuf, 1, 0) < 0)
            {
                return -1;
            }
        }
        else
        {
            flight.departMin = airbuf;
            airbuf[0] = '0';
            if (send(connfd, airbuf, 1, 0) < 0)
            {
                return -1;
            }
            break;
        }
    }   

    bzero(airbuf, MAXBUFFSIZE);
    sprintf(airbuf, "Please input where the flight depart (If you do not want to add a flight, input -1 to abort):");
    if (send(connfd, airbuf, strlen(airbuf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }

    bzero(airbuf, MAXBUFFSIZE);
    if(recv(connfd, airbuf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }
    ret = atoi(airbuf);
    if (ret == -1)
    {
        return 0;
    }
    flight.source = airbuf;

    bzero(airbuf, MAXBUFFSIZE);
    sprintf(airbuf, "Please input the destination of the flight (If you do not want to add a flight, input -1 to abort):");
    if (send(connfd, airbuf, strlen(airbuf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }

    bzero(airbuf, MAXBUFFSIZE);
    if(recv(connfd, airbuf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }
    ret = atoi(airbuf);
    if (ret == -1)
    {
        return 0;
    }
    flight.destination = airbuf;

    while (1)
    {
        bzero(airbuf, MAXBUFFSIZE);
        sprintf(airbuf, "Please input Seat number (A number great than 0. If you do not want to add a flight, input -1 to abort):");
        if (send(connfd, airbuf, strlen(airbuf), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }

        bzero(airbuf, MAXBUFFSIZE);
        if(recv(connfd, airbuf, MAXBUFFSIZE, 0) < 0)
        {
            perror("read data from client");
            return -1;
        } 

        ret = atoi(airbuf);
        if (ret == -1)
        {
            return 0;
        }
        else if (!isdigit(airbuf[0]) || ret <= 0)
        {
            airbuf[0] = '1';
            if (send(connfd, airbuf, 1, 0) < 0)
            {
                return -1;
            }
        }
        else
        {
            flight.seatNum = airbuf;
            airbuf[0] = '0';
            if (send(connfd, airbuf, 1, 0) < 0)
            {
                return -1;
            }
            break;
        }
    }    
    flight.reservedSeatNum = "0";

    /*
    bzero(airbuf, MAXBUFFSIZE);
    sprintf(airbuf, "%s\t%s\t%s\t%s\t%s\t%s\t%s\t%s\n", flight.flightNum.c_str(), flight.departDay.c_str(), 
        flight.departHour.c_str(), flight.departMin.c_str(), flight.source.c_str(), flight.destination.c_str(),
        flight.seatNum.c_str(), flight.reservedSeatNum.c_str());
    */

    std::string sql = "insert into flight (flightNum, departDay, departHour, departMin, source, destination," \
                    "seatNum, reservedSeatNum) values('" + flight.flightNum + "', '" + flight.departDay + "'," \
                    "'" + flight.departHour + "', '" + flight.departMin + "', '" + flight.source + "', '" +
                    flight.destination + "', '" + flight.seatNum + "', '" + flight.reservedSeatNum + "')";
    
    CMysqlSession mysqlsession;
    mysqlsession.connectDB();

    int status = mysqlsession.insertData(sql);

    if (status != 0)
    {
        sprintf(airbuf, "Add flight failed! Maybe depart Day format is wrong or the flight already exist\n");
    }
    else 
    {
        sprintf(airbuf, "Add flight Success!");
    }
    if (send(connfd, airbuf, strlen(airbuf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }

    if (recv(connfd, airbuf, 1, 0) < 0)
    {
        return -1;
    }

    return 0;
}