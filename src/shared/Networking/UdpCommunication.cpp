/*
** EPITECH PROJECT, 2022
** Communication.cpp
** File description:
** .
*/

#include "UdpCommunication.hpp"

// Function passed to communication thread on creation
void udp_communication_main(std::shared_ptr<MessageQueue<Message<ByteBuf>>> incoming, std::shared_ptr<MessageQueue<Message<ByteBuf>>> outgoing,
                            std::shared_ptr<std::atomic<bool>> stopFlag, int port) {
    if (port < 0) { // bind to available port
        LOG("Binding to available port");
        UdpCommunication com(incoming, outgoing, stopFlag);

        // Setup incoming udp packet handler and outgoing packets handler in asio
        com.setup_incoming_handler();
        com.setup_outgoing_handler();
        com.stop_signal_handler();

        // Run asio context
        com.run();
    } else { // Bind to specific port
        LOG("Binding to specified port : " << port);
        UdpCommunication com(incoming, outgoing, stopFlag, port);

        // Setup incoming udp packet handler and outgoing packets handler in asio
        com.setup_incoming_handler();
        com.setup_outgoing_handler();
        com.stop_signal_handler();

        // Run asio context
        com.run();
    }
}

// For use in the server only
UdpCommunication::UdpCommunication(std::shared_ptr<MessageQueue<Message<ByteBuf>>> incoming, std::shared_ptr<MessageQueue<Message<ByteBuf>>> outgoing,
                                   std::shared_ptr<std::atomic<bool>> stopFlag, int port)
    : _sock(_ctxt, asio::ip::udp::endpoint(asio::ip::udp::v4(), port)),
      _outgoingTimer(_ctxt, asio::chrono::milliseconds(OUTGOING_CHECK_INTERVAL)),
      _stopFlag(stopFlag),
      _stopTimer(_ctxt, asio::chrono::seconds(STOP_CHECK_INTERVAL)) {
    this->_incomingMessages = incoming;
    this->_outgoingMessages = outgoing;
}

// For use in the client only
UdpCommunication::UdpCommunication(std::shared_ptr<MessageQueue<Message<ByteBuf>>> incoming, std::shared_ptr<MessageQueue<Message<ByteBuf>>> outgoing,
                                   std::shared_ptr<std::atomic<bool>> stopFlag)
    : _sock(_ctxt, asio::ip::udp::v4()),
      _outgoingTimer(_ctxt, asio::chrono::milliseconds(OUTGOING_CHECK_INTERVAL)),
      _stopFlag(stopFlag),
      _stopTimer(_ctxt, asio::chrono::seconds(STOP_CHECK_INTERVAL)) {
    this->_incomingMessages = incoming;
    this->_outgoingMessages = outgoing;
}

// Handler Method
void UdpCommunication::setup_incoming_handler() {
    this->_sock.async_receive_from(asio::buffer(this->_buffer), this->_endpoint, [this](const asio::error_code& err, std::size_t bytesTransfered) {
        if (!err) {
            auto addr = this->_endpoint.address();
            auto port = this->_endpoint.port();

            // Check packet size
            ByteBuf buffer = ProtocolUtils::convertBuffer(this->_buffer, 1024);
            unsigned short size = ProtocolUtils::packetSize(buffer);

            if (size >= 2) {
                // Remove useless bytes
                buffer.erase(buffer.begin() + size, buffer.end());

                this->push_message(Message(buffer, addr, port));
            }

            // Reset asio buffer
            memset(this->_buffer, 0, 1024);

            // Call incoming handler again
            this->setup_incoming_handler();
        } else {
            // Reset asio buffer
            memset(this->_buffer, 0, 1024);

            std::cerr << "Error performing async_receive_from()" << err << std::endl;
            this->setup_incoming_handler();
        }
    });
}

// Handler Method
void UdpCommunication::setup_outgoing_handler() {
    this->_outgoingTimer = asio::steady_timer(this->_ctxt, asio::chrono::milliseconds(OUTGOING_CHECK_INTERVAL));

    this->_outgoingTimer.async_wait([this](const asio::error_code& err) {
        if (err) {
            ERRORLOG("Error UdpCommunication: " << err.message());
            this->setup_outgoing_handler();
            return;
        }
        std::optional<Message<ByteBuf>> msg;
        char buffer[1024];

        while ((msg = this->pop_message())) {
            auto msgBody = msg->getMsg();

            // Prepare buffer
            int len = msgBody.size();
            memcpy(buffer, &msgBody, len);
            memset(&buffer[len], 0, 1024 - len);

            asio::ip::udp::endpoint target(msg->getAddr(), msg->getPort());
            this->_sock.send_to(asio::buffer(buffer), target);
        }
        this->setup_outgoing_handler();
    });
}

void UdpCommunication::stop_signal_handler() {
    this->_stopTimer = asio::steady_timer(this->_ctxt, asio::chrono::seconds(STOP_CHECK_INTERVAL));

    this->_stopTimer.async_wait([this](const asio::error_code& err) {
        if (err) {
            std::cerr << "Error in stop_signal_handler(): " << err.message() << std::endl;
            this->stop_signal_handler();
            return;
        }

        if (this->getStopFlag())
            this->stop();

        // Re schedule stop signal handler
        this->stop_signal_handler();
    });
}

void UdpCommunication::run() {
    this->_ctxt.run();
}

void UdpCommunication::stop() {
    this->_ctxt.stop();
}

// Access Methods
void UdpCommunication::push_message(Message<ByteBuf> msg) {
    this->_incomingMessages->push(msg);
}

std::optional<Message<ByteBuf>> UdpCommunication::pop_message(void) {
    return this->_outgoingMessages->pop();
}

bool UdpCommunication::getStopFlag() {
    return *this->_stopFlag;
}
