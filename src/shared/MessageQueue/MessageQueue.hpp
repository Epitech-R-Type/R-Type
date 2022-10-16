/*
** EPITECH PROJECT, 2022
** MessageQueue.hpp
** File description:
** .
*/

#pragma once
#include "../../WindowsGuard.hpp"

#include <asio.hpp>
#include <iostream>
#include <mutex>
#include <optional>
#include <queue>

// Used with the messaging queue in order to be able to pass messages
// along with sender info or recipient info
// Add
template <class T>
class Message {
public:
    Message(T msg, asio::ip::address addr, asio::ip::port_type port)
        : _msg(msg),
          _addr(addr),
          _port(port){};

    // << to stream overload
    template <class T2>
    friend std::ostream& operator<<(std::ostream& os, const Message<T2>& msg);

    // << to str overload
    template <class T2>
    friend std::string& operator<<(std::string& str, const Message<T2>& msg);

    // Getters
    asio::ip::address getAddr() const {
        return this->_addr;
    };
    asio::ip::port_type getPort() const {
        return this->_port;
    };
    T getMsg() const {
        return this->_msg;
    };

private:
    T _msg;
    asio::ip::address _addr;
    asio::ip::port_type _port;
};

// Utility operator overload for message
template <class T>
std::ostream& operator<<(std::ostream& os, const Message<T>& msg) {
    os << msg._msg;

    return os;
}

template <class T>
std::string& operator<<(std::string& str, const Message<T>& msg) {
    str = str + msg._msg;

    return str;
}

// Simple thread safe message queue
template <class T>
class MessageQueue {
public:
    MessageQueue();
    ~MessageQueue();

    // Note : Does not implement size() as no code can safely depend on size
    void push(Message<T> el);

    std::optional<Message<T>> pop(void);

    size_t size();

    bool isEmpty();

private:
    std::mutex _mtx;
    std::queue<Message<T>> _queue;
};
