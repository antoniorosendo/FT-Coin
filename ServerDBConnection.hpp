#ifndef SERVERDBCONNECTION_H_
#define SERVERDBCONNECTION_H_

#include <mariadb/conncpp.hpp>
#include <string>

using namespace std;

class ServerDBConnection final
	{
	private:
		string serverIP = "143.106.243.64";
		string serverDatabase = "PooI_25_A02";
		string serverUser = "PooI_25_A02";
		string serverPassword = "873uhgn1XM";
		//string serverPassword = "A";

		sql::Driver *driver = NULL;
		sql::Connection *connection;

	public:
		ServerDBConnection();
		virtual ~ServerDBConnection();
		sql::Connection* getConnection() const;
	};

#endif
