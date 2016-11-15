#include "cadminthread.h"

#define MAXBUFFSIZE 10240

int sendWelcomeMsg(int connfd);

int QueryInformation(int connfd);
int QueryFlightInformation(int connfd);
int QueryReservationInformation(int connfd);
int QueryAllFlights(int connfd);
int QueryFlightInformationByDate(int connfd);
int QueryAllReservation(int connfd);
int QueryReservationsByName(int connfd);

int deleteInformation(int connfd);
int deleteFlightInformation(int connfd);
int deleteFlightInformationByDate(int connfd);
int deleteFlightInformationById(int connfd);
int deleteReservationInformation(int connfd);

int login(int connfd);

char buf[MAXBUFFSIZE];

void* adminThreadFunc(void* param)
{
    int connfd = *(int*)param;
    if (login(connfd) == 0)
    {
        sendWelcomeMsg(connfd);
    }
    
    close(connfd);
    pthread_exit(NULL);
}

int login(int connfd)
{
    bzero(buf, MAXBUFFSIZE);
    
    while(1)
    {
        sprintf(buf, "Please input your username:");
        if (send(connfd, buf, strlen(buf), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }

        bzero(buf, MAXBUFFSIZE);
        if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
        {
            perror("read data from client");
            return -1;
        }

        std::string name = buf;

        sprintf(buf, "Please input your password:");
        if (send(connfd, buf, strlen(buf), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }

        bzero(buf, MAXBUFFSIZE);
        if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
        {
            perror("read data from client");
            return -1;
        }

        std::string passwd = buf;

        CMysqlSession mysqlsession;
        mysqlsession.connectDB();
        std::string sql = "select * from administrator where username=\"" + name + "\" and password=\"" 
                        + passwd + "\"";
        std::vector<std::string> reservations = mysqlsession.executeSQL(sql);
        bzero(buf, MAXBUFFSIZE);
        if (reservations.empty())
        {
            sprintf(buf, "1");
            if (send(connfd, buf, strlen(buf), 0) < 0)
            {
                perror("Send data to client");
                return -1;
            }
            continue;
        }
        else
        {
            sprintf(buf, "0");
            if (send(connfd, buf, strlen(buf), 0) < 0)
            {
                perror("Send data to client");
                return -1;
            }
            break;
        }   
    }

    return 0;
}

int sendWelcomeMsg(int connfd)
{
    bzero(buf, MAXBUFFSIZE);
    sprintf(buf, "Welcome administrator!\nPlease Select action:\n1) Query Flight information\n" \
        "2) Delete flight or user information\n3) Exit\nPress 1, 2 or 3:\n");

    while (1)
    { 
        if (send(connfd, buf, strlen(buf), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }

        bzero(buf, MAXBUFFSIZE);
        if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
        {
            perror("read data from client");
            return -1;
        }

        int ret = atoi(buf);
        //printf("%s\n", buf);
        if (ret == 1 || ret == 2 || ret == 3)
        {
            bzero(buf, MAXBUFFSIZE);
            sprintf(buf, "0");
            if (send(connfd, buf, strlen(buf), 0) < 0)
            {
                return -1;
            }

            if (ret == 1)
            {
                if (QueryInformation(connfd) < 0)
                {
                    return -1;
                }
            }
            else if (ret == 2)
            {
                if (deleteInformation(connfd) < 0)
                {
                    return -1;
                }
            }
            else
            {
                break;
            }
        }
        sprintf(buf, "Please Select action:\n1) Query Flight information\n" \
        "2) Delete flight or user information\n3) Exit\nPress 1, 2 or 3:\n");
    } 

    return 0;
}

int QueryInformation(int connfd)
{
    bzero(buf, MAXBUFFSIZE);
    while (1)
    {
        sprintf(buf, "Please Select which information that you want to query:\n1) Query Flight information\n" \
        "2) Query reservation information\n3) Exit\nPress 1, 2 or 3:\n");

        if (send(connfd, buf, strlen(buf), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }

        bzero(buf, MAXBUFFSIZE);
        if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
        {
            perror("read data from client");
            return -1;
        }

        int ret = atoi(buf);
        if (ret == 1 || ret == 2 || ret == 3)
        {
            sprintf(buf, "0");
            buf[1] = '\0';
            if (send(connfd, buf, strlen(buf), 0) < 0)
            {
                perror("Send data to client");
                return -1;
            }
            
            if (ret == 1)
            {
                if (QueryFlightInformation(connfd) < 0)
                {
                    return -1;
                }
            }
            else if (ret == 2)
            {
                if (QueryReservationInformation(connfd) < 0)
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

int QueryFlightInformation(int connfd)
{   
    bzero(buf, MAXBUFFSIZE);
    while (1)
    {
        sprintf(buf, "Please Select flight information:\n1) All Flight information\n" \
        "2) Flight Information based on Date.\n3) Exit\nPress 1, 2 or 3:\n");

        if (send(connfd, buf, strlen(buf), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }

        bzero(buf, MAXBUFFSIZE);
        if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
        {
            perror("read data from client");
            return -1;
        }

        int ret = atoi(buf);
        if (ret == 1 || ret == 2 || ret == 3)
        {
            sprintf(buf, "0");
            buf[1] = '\0';
            if (send(connfd, buf, strlen(buf), 0) < 0)
            {
                perror("Send data to client");
                return -1;
            }
            
            if (ret == 1)
            {
                if (QueryAllFlights(connfd) < 0)
                {
                    return -1;
                }
            }
            else if (ret == 2)
            {
                if (QueryFlightInformationByDate(connfd) < 0)
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

int QueryAllFlights(int connfd)
{
    CMysqlSession mysqlsession;
    mysqlsession.connectDB();
    //std::vector<std::string> tables = mysqlsession.executeSQL(sql);
    std::string sql = "select * from flight";
    std::vector<Flight> flights = mysqlsession.fetchFlightInfo(sql);

    std::string ret;
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

    if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
    {
        return -1;
    }
    
    if (recv(connfd, buf, 1, 0) < 0)
    {
        return -1;
    }

    return 0;
}

int QueryFlightInformationByDate(int connfd)
{
    bzero(buf, MAXBUFFSIZE);
    sprintf(buf, "Please Select input date:");

    if (send(connfd, buf, strlen(buf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }

    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }

    std::string date = buf;

    CMysqlSession mysqlsession;
    mysqlsession.connectDB();
    //std::vector<std::string> tables = mysqlsession.executeSQL(sql);
    std::string sql = "select * from flight where departDay=\"" + date + "\"";
    std::vector<Flight> flights = mysqlsession.fetchFlightInfo(sql);
    std::string ret;
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

    if (ret.empty())
    {
        ret = "Not found records matching the conditions\n";
    }
    if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
    {
        return -1;
    }
    
    if (recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        return -1;
    }

    return 0;
}

int QueryReservationInformation(int connfd)
{
    bzero(buf, MAXBUFFSIZE);
    while (1)
    {
        sprintf(buf, "Please choose query method:\n1) Query all reservations of a Flight.\n" \
        "2) Query reservations by a passenger's name\n3) Exit\nPress 1, 2 or 3:\n");

        if (send(connfd, buf, strlen(buf), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }

        bzero(buf, MAXBUFFSIZE);
        if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
        {
            perror("read data from client");
            return -1;
        }

        int ret = atoi(buf);
        if (ret == 1 || ret == 2 || ret == 3)
        {
            sprintf(buf, "0");
            buf[1] = '\0';
            if (send(connfd, buf, strlen(buf), 0) < 0)
            {
                perror("Send data to client");
                return -1;
            }
            
            if (ret == 1)
            {
                if (QueryAllReservation(connfd) < 0)
                {
                    return -1;
                }
            }
            else if (ret == 2)
            {
                if (QueryReservationsByName(connfd) < 0)
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

int QueryAllReservation(int connfd)
{
    CMysqlSession mysqlsession;
    mysqlsession.connectDB();
    std::string sql = "select * from flight";
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
        return 0;
    }
    ret = "All flights list as following:\n";
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
    ret += "Please enter row number of flight that your are interested in:";
    if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }

    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }

    int number = atoi(buf);
    if (number <= 0 || number > flights.size())
    {
        ret = "The input row number is invalid.\n";
    }
    else
    {
        sql.clear();
        sql = "select * from flight_reservation where flightId =" + flights[number - 1].flightId;
        std::vector<std::string> results = mysqlsession.executeSQL(sql);
        ret.clear();

        for (int i = 0; i < results.size(); ++i)
        {
            ret += results[i];
            ret += "\n";
        }

        if (ret.empty())
        {
            ret = "Not found records matching the conditions\n";
        }
    }
    
    if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
    {
        return -1;
    }
    
    if (recv(connfd, buf, 1, 0) < 0)
    {
        return -1;
    }
    return 0;
}

int QueryReservationsByName(int connfd)
{
    bzero(buf, MAXBUFFSIZE);
    sprintf(buf, "Please input the passenger's name:");
    if (send(connfd, buf, strlen(buf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }

    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }

    std::string name = buf;

    CMysqlSession mysqlsession;
    mysqlsession.connectDB();
    std::string sql = "select a.passenger, b.id, b.flightNum, b.departDay, " \
    "b.departHour, b.departMin, b.source, b.destination from flight_reservation " \
    "as a join flight as b on a.flightId = b.id where a.passenger= \"" + name + "\"";
    std::vector<std::string> reservations = mysqlsession.executeSQL(sql);   
    std::string ret;

    if (reservations.empty())
    {
        ret = "1";
        if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }
        return 0;
    }

    ret = "All reservation about passenger's name " + name + " list as following:\n";
    for (int i = 0; i < reservations.size(); ++i)
    {
        ret += std::to_string(i + 1);
        ret += "\t";
        ret += reservations[i];
        ret += "\n";
    }

    if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }
    if (recv(connfd, buf, 1, 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }
    return 0;
}

int deleteInformation(int connfd)
{
    bzero(buf, MAXBUFFSIZE);
    while (1)
    {
        sprintf(buf, "Please select which you want to delete:\n1) Delete Flight information.\n" \
        "2) Delete a reservation.\n3) Exit\nPress 1, 2 or 3:\n");

        if (send(connfd, buf, strlen(buf), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }

        bzero(buf, MAXBUFFSIZE);
        if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
        {
            perror("read data from client");
            return -1;
        }

        int ret = atoi(buf);
        if (ret == 1 || ret == 2 || ret == 3)
        {
            sprintf(buf, "0");
            buf[1] = '\0';
            if (send(connfd, buf, strlen(buf), 0) < 0)
            {
                perror("Send data to client");
                return -1;
            }
            
            if (ret == 1)
            {
                if (deleteFlightInformation(connfd) < 0)
                {
                    return -1;
                }
            }
            else if (ret == 2)
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
    bzero(buf, MAXBUFFSIZE);
    while (1)
    {
        sprintf(buf, "Please choose delete Method:\n1) List All Flights.\n" \
        "2) List all flight on same day.\n3) Exit\nPress 1, 2 or 3:\n");

        if (send(connfd, buf, strlen(buf), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }

        bzero(buf, MAXBUFFSIZE);
        if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
        {
            perror("read data from client");
            return -1;
        }

        int ret = atoi(buf);
        if (ret == 1 || ret == 2 || ret == 3)
        {
            sprintf(buf, "0");
            buf[1] = '\0';
            if (send(connfd, buf, strlen(buf), 0) < 0)
            {
                perror("Send data to client");
                return -1;
            }
            
            if (ret == 1)
            {
                if (deleteFlightInformationById(connfd) < 0)
                {
                    return -1;
                }
            }
            else if (ret == 2)
            {
                if (deleteFlightInformationByDate(connfd) < 0)
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
int deleteFlightInformationById(int connfd)
{
    CMysqlSession mysqlsession;
    mysqlsession.connectDB();
    std::string sql = "select * from flight";
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

    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }

    ret.clear();
    int number = atoi(buf);

    if (number <= 0 || number > flights.size())
    {
        ret = "1";
        if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
        {
            perror("Send data to client");
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

        bzero(buf, MAXBUFFSIZE);
        if (recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
        {
            return -1;
        }

        std::string confirm = buf;
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
    
    std::cout << ret << std::endl;
    if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
    {
        return -1;
    }
    if (recv(connfd, buf, 1, 0) < 0)
    {
        return -1;
    }

    return 0;
}

int deleteFlightInformationByDate(int connfd)
{
    bzero(buf, MAXBUFFSIZE);
    sprintf(buf, "Please Select input date:");
    if (send(connfd, buf, strlen(buf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }

    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }

    std::string date = buf;
    CMysqlSession mysqlsession;
    mysqlsession.connectDB();
    std::string sql = "select * from flight where departDay = \"" + date + "\"";
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
        return 0;
    }

    ret = "All flights on" + date + " list as following:\n";
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
    
    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }

    ret.clear();
    int number = atoi(buf);
    if (number <= 0 || number > flights.size())
    {
        ret = "1";
        if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }
        return 0;
    }
    else {
        sql.clear();

        ret = "Delete this flight and all reservations of this flight? (y/n):";
        if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }
        bzero(buf, MAXBUFFSIZE);
        if (recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
        {
            return -1;
        }
        std::string confirm = buf;
        ret.clear();

        if (confirm != "n" && confirm != "y" && confirm != "N" && confirm != "Y")
        {
            ret = "Unknown Operation! Please press y or n!.\n";
        }
        else if (confirm == "n" || confirm == "N")
        {
            ret = "Delete canceled.\n";
        }
        else {
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

    if (recv(connfd, buf, 1, 0) < 0)
    {
        return -1;
    }
            
    return 0;
}

int deleteReservationInformation(int connfd)
{
    bzero(buf, MAXBUFFSIZE);
    sprintf(buf, "Please input the passenger's name:");
    if (send(connfd, buf, strlen(buf), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }

    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }

    std::string name = buf;

    CMysqlSession mysqlsession;
    mysqlsession.connectDB();
    std::string sql = "select a.passenger, b.id, b.flightNum, b.departDay, " \
    "b.departHour, b.departMin, b.source, b.destination from flight_reservation " \
    "as a join flight as b on a.flightId = b.id where a.passenger= \"" + name + "\"";
    std::vector<std::string> reservations = mysqlsession.executeSQL(sql);   
    std::string ret;

    if (reservations.empty())
    {
        ret = "1";
        if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }
        return 0;
    }

    ret = "All reservation about passenger's name " + name + " list as following:\n";
    for (int i = 0; i < reservations.size(); ++i)
    {
        ret += reservations[i];
        ret += "\n";
    }
    ret += "Please enter row number of flight that you want to delete:";
    if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
    {
        perror("Send data to client");
        return -1;
    }

    bzero(buf, MAXBUFFSIZE);
    if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
    {
        perror("read data from client");
        return -1;
    }

    ret.clear();
    int number = atoi(buf);
    if (number <= 0 || number > reservations.size())
    {
        ret = "1";
        if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }
        return 0;
    }
    else {
        sql.clear();
        ret = "Delete this flight and all reservations of this flight? (y/n):";
        if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
        {
            perror("Send data to client");
            return -1;
        }
        bzero(buf, MAXBUFFSIZE);
        if (recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
        {
            return -1;
        }
        std::string confirm = buf;
        ret.clear();

        if (confirm != "n" && confirm != "y" && confirm != "N" && confirm != "Y")
        {
            ret = "Unknown Operation! Please press y or n!.\n";
        }
        else if (confirm == "n" || confirm == "N")
        {
            ret = "Delete canceled.\n";
        }
        else {
            std::string line = reservations[number - 1];
            int pos1 = line.find("\t");
            ++pos1;
            int pos2 = line.find("\t", pos1);
            std::string flightId = line.substr(pos1, pos2 - pos1);

            sql = "delete from flight_reservation where flightId =" + flightId + " and passenger = \"" + name + "\"";
            int status = mysqlsession.executeDelSQL(sql);

            if (status != 0)
            {
                ret = "Delete flight information failed! Please check whether row number is correct.\n";
            }
            else
            {
                ret = "Delete flight information success!\n";
            }
        }
    }
    
    if (send(connfd, ret.c_str(), ret.length(), 0) < 0)
    {
        return -1;
    }
    
    if (recv(connfd, buf, 1, 0) < 0)
    {
        return -1;
    }

    return 0;
}
