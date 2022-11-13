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
void MessageQueue<T>::push(T el) {
    std::lock_guard<std::mutex> lock(this->_mtx);
    this->_queue.push(el);
}

template <class T>
std::optional<T> MessageQueue<T>::pop() {
    std::lock_guard<std::mutex> lock(this->_mtx);
    if (!this->_queue.size())
        return {};
    else {
        auto output = this->_queue.front();
        this->_queue.pop();
        return std::optional(output);
    }
}

template class MessageQueue<Message<std::string>>;
template class MessageQueue<std::string>;
template class MessageQueue<Message<ByteBuf>>;
