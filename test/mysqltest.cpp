#include <mysql/mysql.h>
#include <iostream>
#include <vector>
#include <string>

int main(int argc, char** argv)
{
	std::vector<std::string> tables;
	MYSQL_RES *result;
	MYSQL_ROW row;
	MYSQL *connection, mysql;

	int state;

	mysql_init(&mysql);

	connection = mysql_real_connect(&mysql,"47.88.28.30","root","lf19820416","airline",0,0,0);
	std::cout << "here" << std::endl;
	if (connection == NULL)
	{
		std::cout << mysql_error(&mysql) << std::endl;
		return -1;
	}

	state = mysql_query(connection, "select * from flight");
	if (state !=0)
	{
		std::cout << mysql_error(connection) << std::endl;
		return -1;
	}

	result = mysql_store_result(connection);

	std::cout << "tables: " << mysql_num_rows(result) << std::endl;
	unsigned int num_fields = mysql_num_fields(result);
	while ( ( row=mysql_fetch_row(result)) != NULL )
	{
		std::cout << num_fields << std::endl;
		for (int i = 0; i < num_fields; ++i)
		{
			std::cout << row[i] << " ";
		}
		std::cout << std::endl;
		tables.push_back(row[1]);
	}

	for (std::string tmp : tables)
	{
		std::cout << tmp << std::endl;
	}

	mysql_free_result(result);

	mysql_close(connection);

	return 0;
}

