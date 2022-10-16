#include "TcpClient.hpp"

void tcp_communication_main(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing,
                            std::shared_ptr<std::atomic<bool>> stopFlag) {
    TcpClient com(incoming, outgoing, stopFlag);

    // Setup incoming udp packet handler and outgoing packets handler in asio
    com.connect();

    // com.setup_outgoing_handler();
    // com.stop_signal_handler();
    // Run asio context
    com.run();
}

TcpClient::TcpClient(std::shared_ptr<MessageQueue<std::string>> incoming, std::shared_ptr<MessageQueue<std::string>> outgoing,
                     std::shared_ptr<std::atomic<bool>> stopFlag)
    : _outgoingTimer(_ctxt, asio::chrono::milliseconds(OUTGOING_CHECK_INTERVAL)),
      _stopFlag(stopFlag),
      _stopTimer(_ctxt, asio::chrono::seconds(STOP_CHECK_INTERVAL)),
      _acceptor(_ctxt, asio::ip::tcp::endpoint(asio::ip::tcp::v6(), TCP_PORT)) {
    this->_incomingMessages = incoming;
    this->_outgoingMessages = outgoing;
}

void TcpClient::setupIncomingHandler(std::shared_ptr<asio::ip::tcp::socket> server) {
    server->async_receive(asio::buffer(this->_buffer), [this, server](const asio::error_code& err, std::size_t bytesTransfered) {
        if (!err) {
            auto addr = server->remote_endpoint().address();
            auto port = server->remote_endpoint().port();

            this->push_message(Message(std::string(this->_buffer), addr, port));

            // Reset buffer
            memset(this->_buffer, 0, 1024);
            std::cout << this->_buffer << std::endl;
            // Reset incoming handler
            this->setupIncomingHandler(server);
        } else {
            // Reset buffer
            memset(this->_buffer, 0, 1024);

            // Handle server disconnection
            // if (err.value() == asio::error::eof)
            //     this->remove_peer(server);

            // Reset incoming handler
            this->setupIncomingHandler(server);
        }
    });
}

void TcpClient::connect() {
    asio::io_service io_service;
    // socket creation
    this->_server = std::make_shared<asio::ip::tcp::socket>(asio::ip::tcp::socket(io_service));
    // connection
    this->_server->connect(asio::ip::tcp::endpoint(asio::ip::address::from_string("127.0.0.1"), 5301));

    this->setupIncomingHandler(this->_server);
}

// Context runtime functions
void TcpClient::run() {
    // Execute context
    this->_ctxt.run();
}

void TcpClient::stop() {
    // Stop context
    this->_ctxt.stop();
}

// Access Methods
void TcpClient::push_message(Message<std::string> msg) {
    this->_incomingMessages->push(msg);
}

std::optional<Message<std::string>> TcpClient::pop_message(void) {
    return this->_outgoingMessages->pop();
}

bool TcpClient::getStopFlag() {
    return *this->_stopFlag;
}
