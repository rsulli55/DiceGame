#include "die.h"

namespace qdg {
    std::ostream& operator<<(std::ostream& out, const Die& die) {
        switch(die.color) {
            case Die::Color::white1:
                out << "White1 ";
                break;
            case Die::Color::white2:
                out << "White2 ";
                break;
            case Die::Color::red:
                out << "Red ";
                break;
            case Die::Color::yellow:
                out << "Yellow ";
                break;
            case Die::Color::green:
                out << "Green ";
                break;
            case Die::Color::blue:
                out << "Blue ";
                break;
            default:
                spdlog::debug("Unecpected die color\n");
                break;
        }
        
        switch(die.face) {
            case Die::Face::one:
                out << "1";
                break;
            case Die::Face::two:
                out << "2";
                break;
            case Die::Face::three:
                out << "3";
                break;
            case Die::Face::four:
                out << "4";
                break;
            case Die::Face::five:
                out << "5";
                break;
            case Die::Face::six:
                out << "6";
                break;
            default:
                spdlog::debug("Unexpected die face\n");
                break;
        }

        return out;
    }
    
    sf::Packet& operator>>(sf::Packet& packet, Die& die) {
        int c;
        int f;
        
        packet >> c >> f;

        die.color = static_cast<Die::Color>(c);
        die.face = static_cast<Die::Face>(f);

        return packet;
    }

    sf::Packet& operator<<(sf::Packet& packet, const Die& die) {
        auto c = static_cast<int>(die.color);
        auto f = static_cast<int>(die.face);

        packet << c << f;
        
        return packet;
    }

}

