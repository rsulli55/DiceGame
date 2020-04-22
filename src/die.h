#pragma once

#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>
#include <SFML/Network/Packet.hpp>

#include "game_constants.h"

namespace qdg {
    struct Die {
        enum class Color {
            white1, white2, 
            red, yellow, 
            green, blue\
        };
        
        enum class Face {
            one, two, 
            three, four,
            five, six
        };
        

        Color color;
        Face face;
        bool active;
    };

    constexpr std::array<Die::Color, number_of_dice> all_die_colors {
        Die::Color::white1, Die::Color::white2,
        Die::Color::red, Die::Color::yellow,
        Die::Color::green, Die::Color::blue
    };


       
    std::ostream& operator<<(std::ostream& out, const Die& die);
    sf::Packet& operator>>(sf::Packet& packet, Die& die);
    sf::Packet& operator<<(sf::Packet& packet, const Die& die);
}

