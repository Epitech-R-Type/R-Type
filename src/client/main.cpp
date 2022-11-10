#include "Client.hpp"

Index g_idCounter = 0;

char* getFlagValue(char** begin, char** end, const std::string& option) {
    char** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end) {
        return *itr;
    }
    return 0;
}

bool isFlagSet(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}

int main(int argc, char** argv) {

    if (!isFlagSet(argv, argv + argc, "-v")) {
        SetTraceLogLevel(7);
    }

    Client client;

    if (isFlagSet(argv, argv + argc, "--ip")) {
        std::string ip = getFlagValue(argv, argv + argc, "--ip");
        if (isFlagSet(argv, argv + argc, "--port"))
            client.connect(ip, atoi(getFlagValue(argv, argv + argc, "--port")));
        else
            client.connect(ip);
    }

    client.mainLoop();

    return 0;
}
