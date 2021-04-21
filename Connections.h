//
// Created by Шахова Анастасия on 19.04.2021.
//

#ifndef SEEERVER_CONNECTIONS_H
#define SEEERVER_CONNECTIONS_H


#include <memory>
#include <vector>
#include <boost/asio.hpp>

using namespace boost::asio;

class Connections: public std::enable_shared_from_this<Connections> {

public:

    Connections(io_service & io_service) : client_socket(io_service), server_socket(io_service), data_to_client(8192), data_to_server(8192){};
    ip::tcp::socket & clientSocket() { return client_socket; };
    void startConnection(std::string & server_host, unsigned short server_port);
    void closeConnection();


    void handleConnect(const boost::system::error_code & error);

    void handleWriteToClient(const boost::system::error_code & error);

    void handleWriteToServer(const boost::system::error_code & error);

    void handleReadFromServer(const boost::system::error_code & error, const size_t &bytes_transferred);

    void handleReadFromClient(const boost::system::error_code & error, const size_t &bytes_transferred);

private:

    ip::tcp::socket client_socket;
    ip::tcp::socket server_socket;

    std::vector <unsigned char> data_to_client;
    std::vector <unsigned char> data_to_server;

};

#endif //SEEERVER_CONNECTIONS_H
