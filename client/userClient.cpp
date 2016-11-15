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
#define MAXBUFSIZE 4096
int FlightInformation(int connfd);
int StartBook(int connfd);
int BookFlight(int connfd);
int finish_book(int connfd);
int main(int argc, char** argv)
{
	int clientfd;
	struct sockaddr_in dest_addr;
	char filename[100] = {0};
        char ans[100] = {0};
        char question1[] = {0};

	//if do not have serverIP, server port and number, exit.
	if (argc != 3)
	{
		printf("usage: ./tcpSquareClient serverip serverport.\n eg: ./tcpSquareClient 127.0.0.1 2000\n");
		exit(1);
	}
        printf("begin create the socket\n");
	

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
        printf("connect to server successful!");
        
        StartBook(clientfd);
	
        
	

	close(clientfd);

	return 0;
}
int StartBook(int clientfd)
{
    char buf[MAXBUFSIZE] = {0};
    
    bool breakout=true;
    
    while (breakout){
        bzero(buf, MAXBUFSIZE);
        if (recv(clientfd,buf,MAXBUFSIZE,0)<0){
            //"Welcome to the flight booking center! \nPlease input the following option:\n1. Booking flight \n2. Exit (input the number only)"
            // receive the welcome information from the server
            perror("Receive the information from buffer failed");
            return -1;
        }
        printf("%s",buf);
        bzero(buf,MAXBUFSIZE);
        
        int ans=0;
        scanf("%d",&ans);
        
        sprintf(buf,"%d",ans);
        
        if (send(clientfd, buf, strlen(buf), 0) < 0){
            //send the choice to the server
            perror("Sending filename to server failed\n");  //fail to send the filename
            return -1;
        }
        
        bzero(buf,MAXBUFSIZE);
        if (recv(clientfd,buf,1,0)<0){
            //receive the respond from the server,server validate the option
            perror("Receive the information from buffer failed");
            return -1;
        } 
        int ret = atoi(buf);
        if(ret == 0){
            //server validate, it is a available option
            if (ans==1){
                FlightInformation(clientfd);
                break;
            }
            else{
                break;
            }
        }
        else{
            printf("Invalid input\n");
        }
        
    }
    return 0; 
}

int FlightInformation(int clientfd){
    char buf[MAXBUFSIZE] = {0};
    
    bool breakout=true;
    
    while (breakout){
	bzero(buf,MAXBUFSIZE);
        if (recv(clientfd,buf,MAXBUFSIZE,0)<0){
            //"Please Select flight information: \n1:Show all flight information \n2. Exit"
            // receive the flight information option from the server
            perror("Receive the information from buffer failed");
            return -1;
        }
        printf("%s",buf);
        bzero(buf,MAXBUFSIZE);
        
        int ans=0;
        scanf("%d",&ans);
        sprintf(buf,"%d",ans);
        if (send(clientfd, buf, strlen(buf), 0) < 0){
            //send the choice to the server
            perror("Sending filename to server failed\n");  //fail to send the filename
            return -1;
        }
        
        bzero(buf,MAXBUFSIZE);
        if (recv(clientfd,buf,1,0)<0){
            //receive the respond from the server,server validate the option
            perror("Receive the information from buffer failed");
            return -1;
        } 
	
        int ret = atoi(buf);
        if(ret == 0){
            //server validate, it is a available option
	    bzero(buf,MAXBUFSIZE);
            sprintf(buf,"begin to receive the flight info");
	    if (send(clientfd, buf, strlen(buf), 0) < 0){
                //send the choice to the server
                perror("Sending filename to server failed\n");  //fail to send the filename
                return -1;
            }
            if (ans==1){
		bzero(buf,MAXBUFSIZE);
        	if (recv(clientfd,buf,MAXBUFSIZE,0)<0){
			//receive the respond from the server,server validate the option
                	perror("Receive the information from buffer failed");
                	return -1;
        	}
        	printf("%s",buf); 		
                BookFlight(clientfd);
                break;
            }
	    else if(ans==2){
		bzero(buf,MAXBUFSIZE);
        	if (recv(clientfd,buf,MAXBUFSIZE,0)<0){
			//Ask to input the data to the server
                	perror("Receive the information from buffer failed");
                	return -1;
        	}
		printf("%s",buf);
		bzero(buf,MAXBUFSIZE);
        	char ans1[125]={0};
        	scanf("%s",ans1);
		sprintf(buf,"%s",ans1);		
		if (send(clientfd, buf, strlen(buf), 0) < 0){
                	//send the date to the server
                	perror("Sending filename to server failed\n");  //fail to send the filename
                	return -1;
            	}
		bzero(buf,MAXBUFSIZE);
        	if (recv(clientfd,buf,MAXBUFSIZE,0)<0){
			//Ask to input the data to the server
                	perror("Receive the information from buffer failed");
                	return -1;
        	}
		ans=atoi(buf);
		if(ans!=1)
		{
                        printf("%s",buf);
                        BookFlight(clientfd);
			break;
			
		}
		else{
                        printf("no result match your input data, please try again\n");
		
		}
	    }
            else{
                break;
            }
        }
        else{
            printf("Invalid input\n");
        }
        
    }
    return 0;
    
    
}

int BookFlight(int clientfd){
    char buf[MAXBUFSIZE] = {0};
    
    bool breakout=true;
    
    while (breakout){
        int ans=0;
        scanf("%d",&ans);
	if(ans == -1){
	break;
	}
        bzero(buf,MAXBUFSIZE);
        sprintf(buf,"%d",ans);
        
        if (send(clientfd, buf, strlen(buf), 0) < 0){
            //send the choice to the server
            perror("Sending filename to server failed\n");  //fail to send the filename
            return -1;
        }
        bzero(buf,MAXBUFSIZE);
        if (recv(clientfd,buf,MAXBUFSIZE,0)<0){
            // receive acknowledgement from the server
            perror("Receive the information from buffer failed");
            return -1;
        }
        int ret = atoi(buf);
        
        
        if(ret == 0){
            bzero(buf,MAXBUFSIZE);
            sprintf(buf,"123123");
            if (send(clientfd, buf, strlen(buf), 0) < 0){
               //send the choice to the server
               perror("Sending filename to server failed\n");  //fail to send the filename
               return -1;
            }
            //server validate, it is a available option
            bzero(buf,MAXBUFSIZE);
            if (recv(clientfd,buf,MAXBUFSIZE,0)<0){
               // receive the flight information from the server
               perror("Receive the information from buffer failed");
               return -1;
            }
            printf("%s",buf);
	    bzero(buf,MAXBUFSIZE);
            finish_book(clientfd);
            break;
            
        }
        else{
            printf("The flight was already full or invalid input, please input again,or type -1 to quit\n");
        }
        
        
    }
    return 0;
    
}

int finish_book(int clientfd)
{       char ans1[125]={0};
	char ans2[125]={0};
	char ans3[125]={0};
	char buf[MAXBUFSIZE] = {0};
        printf("your first name: \n");
        scanf("%s",ans1);
	printf("your last name:\n");
	scanf("%s",ans2);
	strcat(ans3,ans1);
	strcat(ans3," ");
	strcat(ans3,ans2);
	bzero(buf,MAXBUFSIZE);
	sprintf(buf,"%s\n",ans3);
        if (send(clientfd,buf,strlen(buf),0)<0){
		//send the choice to the server
		perror("Sending filename to server failed\n");  //fail to send the filename
        	return -1;
        }
	bzero(buf,MAXBUFSIZE);
	if (recv(clientfd,buf,MAXBUFSIZE,0)<0){
		// receive the flight information from the server
        	perror("Receive the information from buffer failed");
        	return -1;
        }
	printf("%s\n",buf);
		
	return 0;
           
            
}
