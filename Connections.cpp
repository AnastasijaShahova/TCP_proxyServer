//
// Created by Шахова Анастасия on 19.04.2021.
//

#include <iostream>
#include "Connections.h"
#include "Log.h"

void Connections::startConnection(std::string & server_host, unsigned short server_port)
{
    auto connection = shared_from_this();
    server_socket.async_connect(
            ip::tcp::endpoint(ip::address::from_string(server_host), server_port),
            [connection](const boost::system::error_code & error)
            {
                connection->handleConnect(error);
            }
    );
}

void Connections::closeConnection()
{
    if (client_socket.is_open())
        client_socket.close();

    if (server_socket.is_open())
        server_socket.close();
}

void Connections::handleConnect(const boost::system::error_code & error)
{
    if (error)
    {
        std::cout << "Error occured ! Error code = " << error.value()
                  << ". Message : " << error.message() << "\n";
        closeConnection();
    }
    else
    {
        auto self_connection = shared_from_this();

        server_socket.async_read_some(
                buffer(data_to_server),
                [self_connection](const boost::system::error_code & error, size_t bytes_transferred)
                {
                    self_connection->handleReadFromServer(error, bytes_transferred);
                }
        );

        client_socket.async_read_some(
                buffer(data_to_client),
                [self_connection](const boost::system::error_code & error, size_t bytes_transferred)
                {
                    self_connection->handleReadFromClient(error, bytes_transferred);
                }
        );
    }

}

void Connections::handleReadFromServer(const boost::system::error_code & error, const size_t & bytes_transferred)
{
    if (error)
    {
        std::cout << "Error occured ! Error code = " << error.value()
                  << ". Message : " << error.message() << "\n";
        closeConnection();
    }
    else
    {
        auto self_connection = shared_from_this();

        async_write(client_socket,
                    boost::asio::buffer(data_to_server, bytes_transferred),
                    [self_connection](const boost::system::error_code& error, size_t size)
                    {
                        self_connection->handleWriteToClient(error);
                    });
    }

}

void Connections::handleReadFromClient(const boost::system::error_code & error, const size_t & bytes_transferred)
{
    if (error)
    {
        std::cout << "Error occured ! Error code = " << error.value()
                  << ". Message : " << error.message() << "\n";
        closeConnection();
    }
    else
    {
        Log::getInstance()->log(data_to_client);

        auto self_connection = shared_from_this();

        async_write(server_socket,
                    buffer(data_to_client, bytes_transferred),
                    [self_connection](const boost::system::error_code & error, size_t size)
                    {
                        self_connection->handleWriteToServer(error);
                    }
        );
    }
}

void Connections::handleWriteToClient(const boost::system::error_code & error)
{
    if (error)
    {
        std::cout << "Error occured ! Error code = " << error.value()
                  << ". Message : " << error.message() << "\n";
        closeConnection();
    }
    else
    {
        auto self_connection = shared_from_this();

        server_socket.async_read_some(
                boost::asio::buffer(data_to_server),
                [self_connection](const boost::system::error_code& error, size_t bytes_transferred)
                {
                    self_connection->handleReadFromServer(error, bytes_transferred);
                }
        );
    }
}

void Connections::handleWriteToServer(const boost::system::error_code & error)
{
    if (error)
    {
        std::cout << "Error occured ! Error code = " << error.value()
                  << ". Message : " << error.message() << "\n";
        closeConnection();
    }
    else
    {
        auto self_connection = shared_from_this();

        client_socket.async_read_some(
                buffer(data_to_client),
                [self_connection](const boost::system::error_code & error, size_t bytes_transferred)
                {
                    self_connection->handleReadFromClient(error, bytes_transferred);
                }
        );
    }
}