/*
** EPITECH PROJECT, 2022
** ConnectionManager.hpp
** File description:
** .
*/

#pragma once

#include <vector>
#include <asio.hpp>
#include <optional>

struct ClientAddr {
    asio::ip::address ip;
    asio::ip::port_type port;
};

struct Client {
    int id;
    ClientAddr addr;
};

class ConnectionManager {
    public:
        ConnectionManager() = default;
        ~ConnectionManager() = default;

        int getClientId(asio::ip::address addr, asio::ip::port_type port) {
            for (auto &client: this->_clients)
                if (client.addr.ip == addr && client.addr.port == port)
                    return client.id;

            Client new_client = {
                this->_clients.size(),
                {addr, port}
            };
            this->_clients.push_back(new_client);

            return new_client.id;
        }

        std::optional<ClientAddr> getClientAddr(int client_id) {
            for (auto &client: this->_clients)
                if (client.id == client_id)
                    return std::optional(client.addr);
            return {};
        }

    private:
        std::vector<Client> _clients;
};
