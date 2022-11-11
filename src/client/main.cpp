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
    Client client;

    if (!isFlagSet(argv, argv + argc, "-v")) {
        Ray::SetTraceLogLevel(7);
    }

    if (isFlagSet(argv, argv + argc, "-ip")) {
        int connectionResult;

        std::string ip = getFlagValue(argv, argv + argc, "-ip");
        if (isFlagSet(argv, argv + argc, "-port"))
            connectionResult = client.connect(ip, atoi(getFlagValue(argv, argv + argc, "-port")));
        else
            connectionResult = client.connect(ip);

        if (connectionResult)
            return 84;
    } else {
        LOG("You need to provide an IP, you can specify a port, defaults to " << TCP_PORT << ".");
        LOG("Use --help for more.");
        return 84;
    }

    client.mainLoop();

    return 0;
}
