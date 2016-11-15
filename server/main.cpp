#include "csession.h"

int main(int argc, char** argv)
{
	if (argc != 5)
	{
		printf("usage: ./airlineReservationServer serverIP airlinePort userPort adminPort\n");
		exit(1);
	}
	int airlinePort = atoi(argv[2]);
	int userPort 	= atoi(argv[3]);
	int adminPort 	= atoi(argv[4]);

	CSession session;
	session.initializeSocket(argv[1], airlinePort, userPort, adminPort);
	session.startSevice();

	return 0;
}