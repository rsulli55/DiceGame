#include <SFML/Network.hpp>

#include "../gameboard.h"

/* TODO */ 
/* When implementing the networking code, it may be good to send Gamestate::Messages */
/* These would consist of a player id and an optional of qdg::Gameboard::Position */
/* and a PlayerPenalty */ 

sf::Packet& operator<<(sf::Packet& packet, const qdg::Gameboard::Position& position) {
    int color = static_cast<int>(position.color);
    int number = static_cast<int>(position.number);

    packet << color << number;

    return packet;
}

sf::Packet& operator>>(sf::Packet& packet, qdg::Gameboard::Position& position) {
    int color;
    int number;

    packet >> color >> number;

    return packet;
}
