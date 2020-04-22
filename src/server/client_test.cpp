#include <fmt/format.h>
#include <SFML/Network.hpp>


int main() {
 

    const auto server_ip = sf::IpAddress(1186864313);
    /* const auto server_ip = sf::IpAddress("192.168.0.249"); */
    const auto server_port = (unsigned short) 54'000;

    qdg::Gameboard gameboard;

    qdg::Gameboard::Position position { 
        qdg::Gameboard::Color::red, qdg::Gameboard::Number::two 
    };


    auto public_ip = sf::IpAddress::getPublicAddress();

    fmt::print("Clients's public IP: {}\n", public_ip.toString());

    auto port = (unsigned short) 54'001;

    fmt::print("Setting up TCP socket\n");
    /* fmt::print("Setting up UDP socket on port {}\n", port); */

    /* sf::UdpSocket socket; */

    /* if (socket.bind(port) != sf::Socket::Done) { */
    /*     fmt::print("Could not bind socket, exiting\n"); */
    /*     exit(1); */
    /* } */

    sf::TcpSocket socket;
    sf::Socket::Status status = socket.connect(server_ip, server_port);
    if (status != sf::Socket::Done) {
        fmt::print("Failed setting up socket, exiting\n");
        exit(1);
    }

    fmt::print("Successfully bound socket\n");

    gameboard.mark_position(position);

    sf::Packet packet;
    packet << position;



    if (socket.send(packet) != sf::Socket::Done) {
        fmt::print("Could not send packet, exiting\n");
        exit(1);
    }

    auto c = static_cast<int>(position.color);
    auto n = static_cast<int>(position.number);


    fmt::print("Successfully sent Gameboard::Position color {} number {}, exiting\n",
            c, n);
    return 0;
}


