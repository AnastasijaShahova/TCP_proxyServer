//
// Created by Шахова Анастасия on 19.04.2021.
//

#include <iostream>
#include "TcpProxy.h"
#include "Connections.h"

TcpProxy::TcpProxy(io_service & io_service, std::string & local_host_address, unsigned short local_port_number,
                   std::string & remote_address, unsigned short remote_port) :
        the_io_service(io_service),
        the_address(ip::address_v4::from_string(local_host_address)),
        local_port(local_port_number),
        remote_address(remote_address),
        remote_port(remote_port),
        the_acceptor(the_io_service, ip::tcp::endpoint(the_address, local_port))
{
}

bool TcpProxy::acceptConnections()
{
    try {
        connections_ptr = std::shared_ptr<Connections>(new Connections(the_io_service));

        the_acceptor.async_accept(connections_ptr->clientSocket(),
                                  [this](const boost::system::error_code & error)
                               {
                                   handleAccept(error);
                               }
        );
    }
    catch (std::exception & e)
    {
        std::cerr << e.what() << std::endl;
        return false;
    }

    return true;
}

void TcpProxy::handleAccept(const boost::system::error_code & error)
{
    if (error)
    {
        std::cerr << error.message() << std::endl;
    }
    else
    {
        std::cout << "Accepted!!!!!!" << std::endl;
        connections_ptr->startConnection(remote_address, remote_port);
        if (!acceptConnections())
        {
            std::cerr << "Failed  connection" << std::endl;
        }

    }
}