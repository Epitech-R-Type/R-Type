#include <iostream>
#include <asio.hpp>

#include "Server.hpp"

int main() {
    Server serv;
    serv.setup();
    serv.main_loop();

    return 0;
}
