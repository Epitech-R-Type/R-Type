/*
** EPITECH PROJECT, 2022
** MessageQueue.hpp
** File description:
** .
*/

#pragma once

#include <mutex>
#include <optional>
#include <queue>

// This will have to be moved
struct Message {
    int client_id;
    std::string msg;
};

// Simple thread safe message queue
template <class T>
class MessageQueue {
public:
    MessageQueue();
    ~MessageQueue();

    // Note : Does not implement size() as no code can safely depend on size
    void push(T el);
    std::optional<T> pop(void);

private:
    std::mutex _mtx;
    std::queue<T> _queue;
};
