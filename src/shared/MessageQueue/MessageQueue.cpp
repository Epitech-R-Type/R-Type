/*
** EPITECH PROJECT, 2022
** MessageQueue.cpp
** File description:
** .
*/

#include "MessageQueue.hpp"

template <class T>
MessageQueue<T>::MessageQueue() {}

template <class T>
MessageQueue<T>::~MessageQueue() {}

template <class T>
void MessageQueue<T>::push(Message<T> el) {
    std::lock_guard<std::mutex> lock(this->_mtx);
    this->_queue.push(el);
}

template <class T>
std::optional<Message<T>> MessageQueue<T>::pop() {
    std::lock_guard<std::mutex> lock(this->_mtx);
    if (!this->_queue.size())
        return {};
    else {
        auto output = this->_queue.front();
        this->_queue.pop();
        return std::optional(output);
    }
}

template <class T>
size_t MessageQueue<T>::size() {
    return this->_queue.size();
};

template <class T>
bool MessageQueue<T>::isEmpty() {
    return this->_queue.size() == 0;
};

template class MessageQueue<std::string>;
