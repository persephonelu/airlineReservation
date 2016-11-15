#include <iostream>
#include <vector>
#include "cmysqlsession.h"
#include <string>

using namespace std;

int main(int argc, char** argv)
{
	CMysqlSession* session = new CMysqlSession();
	session->connectDB();
	vector<string> tables = session->executeSQL("show databases");
	for (string str : tables)
	{
		cout << str << endl;
	}
	return 0;
}
