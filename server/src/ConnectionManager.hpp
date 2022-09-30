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
            for (auto &client: this->_clients) {
                if (client.addr == addr && client.port == port)
                    return client.id;
                else {
                    Client new_client = {
                        this->_clients.size(),
                        addr,
                        port
                    };
                    this->_clients.push_back(new_client);

                    return new_client.id;
                }
            }
        }

    private:
        std::vector<Client> _clients;
};
