#include "cairlinethread.h"
#include "cflight.h"

#define MAXBUFFSIZE 10240

char airbuf[MAXBUFFSIZE];

int welcome(int connfd, int& userid);
int companySignup(int connfd, int& userid);
int companySignin(int connfd, int& userid);
int startService(int connfd, int& userid);
int listAllFlight(int connfd, int& userid);
int addFlight(int connfd, int& userid);
int delFlight(int connfd, int& userid);

void* airlineThreadFunc(void* param)
{
    int connfd = *(int*)param;
    int userid = 0;
    if (welcome(connfd, userid) == 0)
    {
        startService(connfd, userid);
    }

    close(connfd);
    pthread_exit(NULL);
}

int welcome(int connfd, int& userid)
{
    bzero(airbuf, MAXBUFFSIZE);
    sprintf(airbuf, "Welcome airline company!\nPlease select your action:\n1) Registration\n" \
        "2) Sign in\n3) Exit\nPress 1, 2 or 3:\n");
    while(1)
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

        if (ret == 1 || ret == 2 || ret == 3)
        {
            bzero(airbuf, MAXBUFFSIZE);
            sprintf(airbuf, "0");
            if (send(connfd, airbuf, strlen(airbuf), 0) < 0)
            {
                return -1;
            }

            if (ret == 1)
            {
                if (companySignup(connfd, userid) == 0)
                {
                    break;
                }
            }
            else if (ret == 2)
            {
                if (companySignin(connfd, userid) == 0)
                {
                    break;
                }
            }
            else
            {
                return 1;
            }
        }
        sprintf(airbuf, "Please select your action:\n1) Registration\n" \
        "2) Sign in\n3) Exit\nPress 1, 2 or 3:\n");
    }

    return 0;
}

int companySignup(int connfd, int& userid)
{
    bzero(airbuf, MAXBUFFSIZE);

    sprintf(airbuf, "Please input your username:");
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

    std::string name = airbuf;
    bzero(airbuf, MAXBUFFSIZE);
    sprintf(airbuf, "Please input your password:");
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

    std::string passwd = airbuf;

    CMysqlSession mysqlsession;
    mysqlsession.connectDB();
        
    std::string sql = "insert into airline_company (username, password) values('" + name + "', '" + passwd + "')";
    int status = mysqlsession.insertData(sql);

    if (status != 0)
    {
        airbuf[0] = '1';
    }
    else 
    {
        sql = "select * from airline_company where username=\"" + name + "\" and password=\"" 
                        + passwd + "\"";
        std::vector<std::string> reservations = mysqlsession.executeSQL(sql);
        int pos = reservations[0].find("\t");
        userid = atoi(reservations[0].substr(0, pos).c_str());
        //printf("%d\n", userid);
        airbuf[0] = '0';
    }

    if (send(connfd, airbuf, 1, 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }

    return status == 0 ? 0 : -1;
}

int companySignin(int connfd, int& userid)
{
    bzero(airbuf, MAXBUFFSIZE);  

    sprintf(airbuf, "Please input your username:");
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

    std::string name = airbuf;

    sprintf(airbuf, "Please input your password:");
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

    std::string passwd = airbuf;

    CMysqlSession mysqlsession;
    mysqlsession.connectDB();
    std::string sql = "select * from airline_company where username=\"" + name + "\" and password=\"" 
                        + passwd + "\"";
    std::vector<std::string> reservations = mysqlsession.executeSQL(sql);
    bzero(airbuf, MAXBUFFSIZE);
    if (reservations.empty())
    {
        sprintf(airbuf, "1");
        if (send(connfd, airbuf, 1, 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }
        return -1;
    }
    else
    {
        sprintf(airbuf, "0");
        if (send(connfd, airbuf, 1, 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }
        int pos = reservations[0].find("\t");
        userid = atoi(reservations[0].substr(0, pos).c_str());
        //printf("%d\n", userid);
    }   

    return 0;
}

int startService(int connfd, int& userid)
{
    bzero(airbuf, MAXBUFFSIZE);
    sprintf(airbuf, "Welcome!\nPlease Select action:\n1) List All flights.\n2) Add a flight.\n" \
        "3) Delete a flight.\n4) Exit\nPress 1, 2, 3 or 4:\n");

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
        //printf("%s\n", airbuf);
        if (ret == 1 || ret == 2 || ret == 3 || ret == 4)
        {
            bzero(airbuf, MAXBUFFSIZE);
            sprintf(airbuf, "0");
            if (send(connfd, airbuf, strlen(airbuf), 0) < 0)
            {
                return -1;
            }

            if (ret == 1)
            {
                if (listAllFlight(connfd, userid) < 0)
                {
                    return -1;
                }
            }
            else if (ret == 2)
            {
                if (addFlight(connfd, userid) < 0)
                {
                    return -1;
                }
            }
            else if (ret == 3)
            {
                if (delFlight(connfd, userid) < 0)
                {
                    return -1;
                }
            }
            else if (ret == 4)
            {
                break;
            }
        }
        bzero(airbuf, MAXBUFFSIZE);
        sprintf(airbuf, "Please Select action:\n1) List All flights.\n2) Add a flight.\n" \
        "3) Delete a flight.\n4) Exit\nPress 1, 2, 3 or 4:\n");
    } 

    return 0;
}

int listAllFlight(int connfd, int& userid)
{
    CMysqlSession mysqlsession;
    mysqlsession.connectDB();
    //std::vector<std::string> tables = mysqlsession.executeSQL(sql);
    std::string sql = "select * from flight where companyId=" + std::to_string(userid);
    std::vector<Flight> flights = mysqlsession.fetchFlightInfo(sql);

    std::string ret;
    if (!flights.empty())
    {
        for (int i = 0; i < flights.size(); ++i)
        {
            ret += std::to_string(i + 1);
            ret += "\t";
            ret += flights[i].flightNum;
            ret += "\t";
            ret += flights[i].departDay + " " + flights[i].departHour + ":" + flights[i].departMin + ":00";
            ret += "\t";
            ret += flights[i].source;
            ret += "\t";
            ret += flights[i].destination;
            ret += "\t";
            ret += flights[i].seatNum;
            ret += "\t";
            ret += flights[i].reservedSeatNum;
            ret += "\n";
        }
    }
    else
    {
        ret = "Not found flight recorders\n";
    }

    if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
    {
        return -1;
    }
    if (recv(connfd, airbuf, 1, 0) < 0)
    {
        return -1;
    }
    return 0;
}

int addFlight(int connfd, int& userid)
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
                    "seatNum, reservedSeatNum, companyId) values('" + flight.flightNum + "', '" + flight.departDay + "'," \
                    "'" + flight.departHour + "', '" + flight.departMin + "', '" + flight.source + "', '" +
                    flight.destination + "', '" + flight.seatNum + "', '" + flight.reservedSeatNum + "', '" 
                    + std::to_string(userid) + "')";
    
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

int delFlight(int connfd, int& userid)
{
    CMysqlSession mysqlsession;
    mysqlsession.connectDB();
    std::string sql = "select * from flight where companyId=" + std::to_string(userid);
    std::vector<Flight> flights = mysqlsession.fetchFlightInfo(sql);

    std::string ret;
    if (flights.empty())
    {
        ret = "1";
        if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
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

    ret += "All flights list as following:\n";
    for (int i = 0; i < flights.size(); ++i)
    {
        ret += std::to_string(i + 1);
        ret += "\t";
        ret += flights[i].flightNum;
        ret += "\t";
        ret += flights[i].departDay + " " + flights[i].departHour + ":" + flights[i].departMin + ":00";
        ret += "\t";

        ret += flights[i].source;
        ret += "\t";
        ret += flights[i].destination;
        ret += "\t";
        ret += flights[i].seatNum;
        ret += "\t";
        ret += flights[i].reservedSeatNum;
        ret += "\n";
    }
    ret += "Please enter row number of flight that you want to delete:";
    if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
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

    ret.clear();
    int number = atoi(airbuf);
    //printf("number is %d\n", number);
    if (number <= 0 || number > flights.size())
    {
        ret = "1";
        if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
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
    else {
        sql.clear();

        ret = "Delete this flight and all reservations of this flight? (y/n):";
        if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
        {
            return -1;
        }

        bzero(airbuf, MAXBUFFSIZE);
        if (recv(connfd, airbuf, MAXBUFFSIZE, 0) < 0)
        {
            return -1;
        }

        std::string confirm = airbuf;
        ret.clear();

        if (confirm != "n" && confirm != "y" && confirm != "N" && confirm != "Y")
        {
            ret = "Unknown Operation! Please press y or n!.\n";
        }
        else if (confirm == "n" || confirm == "N")
        {
            ret = "Delete canceled.\n";
        }
        else{
            sql = "start transaction";
            int status = mysqlsession.executeDelSQL(sql);
            int status1 = 0;
            int status2 = 0;

            if (status != 0)
            {
                ret = "Delete flight information failed! Please try again.\n";
            }
            else
            {
                sql = "delete from flight where id=" + flights[number - 1].flightId; 
                status1 = mysqlsession.executeDelSQL(sql);

                sql = "delete from flight_reservation where flightId = " + flights[number - 1].flightId;
                status2 = mysqlsession.executeDelSQL(sql);
            }

            if (status1 != 0 || status2 != 0)
            {
                sql = "rollback";
                status = mysqlsession.executeDelSQL(sql);
                ret = "Delete flight information failed! Please check whether row number is correct.\n";
            }
            else
            {
                sql = "commit";
                status = mysqlsession.executeDelSQL(sql);
                ret = "Delete flight information success!\n";
            }
        }
    }

    if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
    {
        return -1;
    }
    if (recv(connfd, airbuf, 1, 0) < 0)
    {
        return -1;
    }

    return 0;
}