#include <iostream>
#include "ServerDBConnection.hpp"
#include "Utils.hpp"

ServerDBConnection::ServerDBConnection()
{
    try
    {
        this->driver = sql::mariadb::get_driver_instance();
        string connString = "jdbc:mariadb://" + serverIP + "/" + serverDatabase;
        sql::SQLString url(connString);
        sql::Properties properties(
            {
                { "user", this->serverUser },
                { "password", this->serverPassword }
            });

        this->connection = driver->connect(url, properties);

        if (!this->connection || !this->connection->isValid()) {
            this->connection = nullptr;
            throw std::runtime_error("Connection object is invalid.");
        }

        Utils::printFramedMessage("\033[32mConnected to 'MariaDB' RDBMS at " + serverIP + "/" + serverDatabase + "\033[0m", "*", 80);
    }
    catch (sql::SQLException &myException)
    {
        std::cerr << "\033[31mError Connecting to MariaDB Platform: \n\033[0m" << myException.what() << std::endl;
        this->connection = nullptr;
    }
    catch (const std::exception &e)
    {
        std::cerr << "Unexpected error: " << e.what() << std::endl;
        this->connection = nullptr;
    }
}

ServerDBConnection::~ServerDBConnection()
{
    if (this->connection) {
        try {
            this->connection->close();
        } catch (const sql::SQLException &e) {
            std::cerr << "Erro ao fechar a conexão: " << e.what() << std::endl;
        }
    }
}

sql::Connection* ServerDBConnection::getConnection() const
{
    return this->connection;
}