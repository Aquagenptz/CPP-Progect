// MySqlTest.cpp : Defines the entry point for the console application.
//

#include <mysql.h>
#include <iostream>
#include <boost/program_options/detail/config_file.hpp>
#include <boost/program_options/parsers.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/ini_parser.hpp>

using namespace std;
namespace pod = boost::program_options::detail;
int qstate;

int main()
{
	
	// dafault settings connect to db if error read config.ini

	string host_name = "localhost";
	string user_name = "root";
	string password = "";
	string db_name = "";
    unsigned int port = 3306;

	char* host = &host_name[0];
	char* user = &user_name[0];
	char* pass = &password[0];
	char* db = &db_name[0];
	
	
	
	// Read setting info from config.ini
	
	try {
		
		boost::property_tree::ptree pt;
		boost::property_tree::ini_parser::read_ini("config.ini", pt);
	  
		// Set data connect to database from confif.ini file

		host_name = pt.get<std::string>("db_logging.host_name");
		user_name = pt.get<std::string>("db_logging.user_name");
		password = pt.get<std::string>("db_logging.password");
		db_name = pt.get<std::string>("db_logging.db_name");
		
		port = (unsigned int)atoi(pt.get<std::string>("db_logging.port_n").c_str());
	
	}
	catch (exception& e) {
	std:: cout << "Error config file - " << e.what() << std::endl;
	std:: cout << "The default settings for connecting to the database are set!" << std::endl;
		
	}

	

	//Configure and connect to database
	MYSQL* conn;
	MYSQL_ROW row;
	MYSQL_RES* res;


	if ((conn = mysql_init(NULL)) == NULL)
	{
		std::cout << ("mysql_init()") << std::endl;
		exit(EXIT_FAILURE);
	}

	conn = mysql_real_connect(conn, host, user, pass, db, port, NULL, 0);

	
	

	if (conn) {
		puts("Successful connection to database!");

		string query = "SELECT * FROM test";
		const char* q = query.c_str();
		qstate = mysql_query(conn, q);
		if (!qstate)
		{
			res = mysql_store_result(conn);
			while (row = mysql_fetch_row(res))
			{
				printf("ID: %s, Name: %s, Value: %s\n", row[0], row[1], row[2]);
			}
		}
		else
		{
			cout << "Query failed: " << mysql_error(conn) << endl;
		}
	}
	else {
		puts("Connection to database has failed!");
	}

	system("pause");
	return 0;
}
