#include "cuserthread.h"

#define MAXBUFFSIZE 2048
int sendwelcome(int connfd);
int AskFlightInf(int connfd);
int dealBook(int connfd,std::vector<Flight>& flights);
void* userThreadFunc(void* param)
{   
    int connfd = *(int*)param;
    sendwelcome(connfd);
    close(connfd);
    pthread_exit(NULL);
}

int sendwelcome(int connfd){
    char buf[MAXBUFFSIZE] = {0};
    
    bool breakout=true;
    while(breakout)
    {   bzero(buf,MAXBUFFSIZE);
        sprintf(buf,"Welcome to the flight booking center! \nPlease input the following option:\n1. Booking flight \n2. Exit (input the number only)\n"); 
        
        if (send(connfd, buf, strlen(buf), 0) <0){
            perror("send data to client");
            pthread_exit(NULL);
            
        }
        
        bzero(buf,MAXBUFFSIZE);
        if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
        {
            perror("read data from client");
            pthread_exit(NULL);
        }
        
        int ret = atoi(buf);
        //get respond from the user, which option will user choose
        
        if (ret==1||ret==2)
        {
            bzero(buf,MAXBUFFSIZE);
            sprintf(buf,"0");
            buf[1]='\0';
            send(connfd,buf,strlen(buf),0);//send receive acknowledge to client, the client input the right option
            
            if (ret == 1)
            {
                AskFlightInf(connfd);  //send flight information to the client
                break;
            }
            else{
                break;
            }
        }
        else{
            bzero(buf,MAXBUFFSIZE);
            sprintf(buf,"1");
            buf[1]='\0';
            send(connfd,buf,strlen(buf),0);//send negative receive acknowledge to client, the client input the wrong option
            
        }
    
    }
    return 0;
    
}

int AskFlightInf(int connfd){
    char buf[MAXBUFFSIZE] = {0};
    
    bool breakout=true;
    while(breakout){
        bzero(buf,MAXBUFFSIZE);
        sprintf(buf,"Please Select flight information: \n1:Show all flight information \n2.Select flight information by date\n3.Exit\n");
        if (send(connfd, buf, strlen(buf), 0) <0){
            perror("send data to client");
            break;
        }
        
        bzero(buf,MAXBUFFSIZE);
        if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
        {
            perror("read data from client");
            break;
        }
        
        int ret1 = atoi(buf);
        //get respond from the user, which option will user choose
        
        if (ret1==1||ret1==2||ret1==3)
        {
            bzero(buf,MAXBUFFSIZE);
            sprintf(buf,"0");  
            buf[1]='\0';
            send(connfd,buf,strlen(buf),0);//send receive acknowledge to client, the client input the right option
            bzero(buf,MAXBUFFSIZE);
            if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
            {
            //client receive the acknowledgement
            	perror("read data from client");
            	break;
            }
	    
	    bzero(buf,MAXBUFFSIZE);
            if (ret1 == 1)
            { //send all the flight information
		CMysqlSession mysqlsession;
    		mysqlsession.connectDB();
    		std::string sql="select * from flight";
    		std::vector<Flight> flights =mysqlsession.fetchFlightInfo(sql);
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
                ret += "Please Select which flight you would like to choose (input the row number like 1 or 2 or 3):\n";
                
                send(connfd, ret.c_str(), ret.length(), 0);

                dealBook(connfd,flights);
                break;
                
            }
	    else if(ret1 == 2){
                bzero(buf,MAXBUFFSIZE);
		sprintf(buf,"Which day you would like to choose: (please input following 2016-12-10)\n");
		if (send(connfd,buf,strlen(buf),0)<0){
			perror("read data from client");
            		break;
		}
		bzero(buf,MAXBUFFSIZE);  
                if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
                {
                //receive ans
            		perror("read data from client");
            		break;
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
        		send(connfd, ret.c_str(), ret.length(), 0);
        		printf("NO result match your input, please try again\n");
    		}
                else{
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
    			ret += "Please enter row number of flight that you want to select:";
    			send(connfd, ret.c_str(), ret.length(), 0);
			dealBook(connfd,flights);
			break;
            	}
	    }
            
            else{
                break;
            }
        }
        else{
            bzero(buf,MAXBUFFSIZE);
            sprintf(buf,"1");  
            buf[1]='\0';
            send(connfd,buf,strlen(buf),0);//send negative receive acknowledge to client, the client input the right option
        }
        
    }
    return 0;
    
    
}

int dealBook(int connfd,std::vector<Flight>& flights){
    
    bool breakout = true;
    char buf[MAXBUFFSIZE]={0};
    char ans1[MAXBUFFSIZE]={0};
    while(breakout){
     
	bzero(buf,MAXBUFFSIZE);
        if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
        {
            perror("read data from client");
            break;
        }
        
        int ans =atoi(buf);
	//printf("the flight size is :%d",int (flights.size()));
	//printf("the reser set num: %d, set num: %d",int(flights[ans-1].reservedSeatNum),int(flights[ans-1].seatNum));
        if (0<ans<=flights.size()&&atoi(flights[ans-1].reservedSeatNum.c_str())<atoi(flights[ans-1].seatNum.c_str()))
        {
            bzero(buf,MAXBUFFSIZE);
            sprintf(buf,"0");  
            buf[1]='\0';
            send(connfd,buf,strlen(buf),0);//send receive acknowledge to client, the client input the right option
            //update the flight table, increase the reservedseatnum
            bzero(buf,MAXBUFFSIZE);
            if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
            {   //almost finish booking
                perror("read data from client");
                break;
            }
            
            bzero(buf,MAXBUFFSIZE);
            sprintf(buf,"please type your name: \n"); 
            if (send(connfd, buf, strlen(buf), 0) <0){
                perror("send data to client");
                break;
            }
            bzero(buf,MAXBUFFSIZE);
            if(recv(connfd, buf, MAXBUFFSIZE, 0) < 0)
            {
                perror("read data from client");
                break;
            }
	    printf("the passenger name is: %s\n", buf);
            sprintf(ans1,"%s",buf);
	    CMysqlSession mysqlsession;
            mysqlsession.connectDB();
    
            std::string sql="update flight set reservedSeatNum=reservedSeatNum+1 where id="+flights[ans-1].flightId;
            if (mysqlsession.updateData(sql)!=0){
                perror("update database failed");
                break;
            }
            printf("update the reservedSeatNum in ID:%s successful\n",flights[ans-1].flightId.c_str());
            
            sql.clear();
            std::string ret;
            ret += flights[ans-1].flightId;
            sql="insert into flight_reservation values ('"+flights[ans-1].flightId+"','"+ans1+"')";
            if (mysqlsession.insertData(sql)!=0){
                perror("update database failed");
                break;
            }
            printf("update the flight_reservation table in ID:%s, Passenger:%s successful\n",ret.c_str(),buf);
            bzero(buf,MAXBUFFSIZE);
            sprintf(buf,"Finish booking successful!\n"); 
            if (send(connfd, buf, strlen(buf), 0) <0){
                perror("send data to client");
                break;
            }	    
            break;

	
            
        }
        else{
            bzero(buf,MAXBUFFSIZE);
            sprintf(buf,"1");  
            buf[1]='\0';
            send(connfd,buf,strlen(buf),0);//send negative receive acknowledge to client, the client input the right option
        }
     } 
	return 0;   
}


