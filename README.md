Airline Reservation

Airline reservation system is a platform, where airlines sell their flights to others and passengers buy tickets. The system contains airline schedules, passenger reservations and ticket records. An airline can directly push out their flight information to this system; customers make their own reservation through Internet.

For this Project, the airline reservation system includes four parts
1) The Airline Reservation Server is a concurrent connection-oriented server, which provides services to Airline companies, passengers and system administrators at the same time. 
2) An airline company client adds a flight, list all flights of the company or delete a flight of the company.
3) A customer client chooses flights and sends information of customers to the airline reservation server to book a ticket. 
4) A System administrator client can query information of flights and reservations. Meanwhile, it also can delete flights or reservations. The system administrator client sends inquires or removing information to the airline reservation server.

How to use

1) Prerequisite
To build the system, you need install mysql-connect-c and c++11 support.

2) Usage
  1: Enter the directory "server", input "make"
  2: Run a command "./airlineReservationServer 127.0.0.1 2000 2001 2002", "127.0.0.1" is the server IP, "2000" is the port for airline company client, "2001" is the port for passenger client and "2002" is the port for administrators.
  3: Open another terminal, Enter the directory "client", input “make” 
  4: There are three types of the client.
  First airline company client. run "./airlineClient 127.0.0.1 2000", "127.0.0.1" is server IP, “2000” is server port for the airline company client.
  Second passenger client. run "./userClient 127.0.0.1 2001", "127.0.0.1" is server IP, “2001” is server port for the passenger client.
  Third administrator client. run "./adminClient 127.0.0.1 2002", "127.0.0.1" is server IP, “2002” is server port for the administrator client.
