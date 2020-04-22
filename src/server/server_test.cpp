#include <fmt/format.h>
#include <SFML/Network.hpp>






int main() {

    qdg::Gameboard gameboard;

    qdg::Gameboard::Position position;

    auto public_ip = sf::IpAddress::getPublicAddress();

    fmt::print("Server's public IP: {}\n", public_ip.toString());
    fmt::print("Server's public IP as int: {}\n", public_ip.toInteger());

    auto port = (unsigned short) 54'000;

    fmt::print("Setting up TCP listener on port {}\n", port);

    sf::TcpListener listener;

    if (listener.listen(port) != sf::Socket::Done) {
        fmt::print("Failed setting up listener, exiting\n");
        exit(1);
    }

    fmt::print("Trying to accept a client\n");
    sf::TcpSocket client;
    if (listener.accept(client) != sf::Socket::Done) {
        fmt::print("Could not accept client, exiting\n");
        exit(1);
    }
    /* fmt::print("Setting up UDP socket on port {}\n", port); */

    /* sf::UdpSocket socket; */

    /* if (socket.bind(port) != sf::Socket::Done) { */
    /*     fmt::print("Could not bind socket, exiting\n"); */
    /*     exit(1); */
    /* } */

    fmt::print("Successfully bound socket\n");

    sf::Packet packet;

    auto client_ip = sf::IpAddress(1186864313);
    auto client_port = (unsigned short) 54'001;


    if (client.receive(packet) != sf::Socket::Done) {
        fmt::print("Could not receive packet, exiting\n");
        exit(1);
    }

    packet >> position;

    auto c = static_cast<int>(position.color);
    auto n = static_cast<int>(position.number);

    fmt::print("Successfully received Gameboard::Position color {} number {}, exiting\n",
            c, n);
    return 0;
}


