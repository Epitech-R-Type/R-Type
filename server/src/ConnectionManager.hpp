/*
** EPITECH PROJECT, 2022
** ConnectionManager.hpp
** File description:
** .
*/

#pragma once

#include <vector>
#include <asio.hpp>

struct Client {
    int id;
    asio::ip::address addr;
    asio::ip::port_type port;
};

class ConnectionManager {
    public:
        ConnectionManager() = default;
        ~ConnectionManager() = default;

        int getClientId(asio::ip::address addr, asio::ip::port_type port) {
            std::cout << "Incoming connection: " << addr << ":" << port << std::endl;
            for (auto &client: this->_clients) {
                std::cout << "Compared connection: " << client.addr << ":" << client.port << std::endl;
                
                if (client.addr == addr && client.port == port)
                    return client.id;
            }
            
            Client new_client = {
                this->_clients.size(),
                addr,
                port
            };
            this->_clients.push_back(new_client);

            return new_client.id;
        }

    private:
        std::vector<Client> _clients;
};
