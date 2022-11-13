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
    getID<SoundCreation::Component>();
    getID<SoundDestruction::Component>();
    getID<SoundDamage::Component>();
}

bool Utilities::isPortAvailable(int port) {
    asio::io_service svc;
    asio::ip::tcp::acceptor a(svc);
    asio::ip::udp::socket b(svc);

    asio::error_code ec1;
    a.open(asio::ip::tcp::v4(), ec1) || a.bind(asio::ip::tcp::endpoint(asio::ip::tcp::v4(), port), ec1);

    if (ec1 == asio::error::address_in_use)
        return false;

    asio::error_code ec2;
    b.open(asio::ip::udp::v4(), ec2) || b.bind(asio::ip::udp::endpoint(asio::ip::udp::v4(), port), ec2);

    if (ec2 == asio::error::address_in_use)
        return false;

    return true;
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
