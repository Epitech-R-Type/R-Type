#include "Utilities.hpp"
#include "../ECS/Components.hpp"
#include "../ECS/ECS.hpp"
#include <complex>

void Utilities::createCompPoolIndexes() {
    getID<Armor::Component>();
    getID<Health::Component>();
    getID<Position::Component>();
    getID<Animation::Component>();
    getID<Velocity::Component>();
    getID<Player::Component>();
    getID<Damage::Component>();
    getID<Armament::Component>();
    getID<Hitbox::Component>();
    getID<Team::Component>();
    getID<ImmunityFrame::Component>();
    getID<CollisionEffect::Component>();
}

bool Utilities::isPortAvailable(int port) {
    asio::io_service svc;
    asio::ip::tcp::acceptor a(svc);

    asio::error_code ec;
    a.open(asio::ip::tcp::v4(), ec) || a.bind(asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port), ec);

    return ec != asio::error::address_in_use;
}

double Utilities::toRadians(double degree) {
    constexpr double pi = 22 / 7;
    return (double)degree * (pi / 180.0);
}

Point Utilities::rotate(Point point, Point origin, double rad) {
    std::complex<double> P = {point.x, point.y};
    std::complex<double> Q = {origin.x, origin.y};

    std::complex<double> res = (P - Q) * std::polar(1.0, rad) + Q;
    return {
        res.real(),
        res.imag(),
    };
}

std::vector<std::string> Utilities::splitStr(std::string str, std::string sep) {
    size_t pos = 0;
    std::string token;
    std::vector<std::string> output;

    while ((pos = str.find(sep)) && pos != std::string::npos) {
        output.push_back(str.substr(0, pos));
        str.erase(0, pos + sep.length());
    }
    output.push_back(str);
    return output;
}
