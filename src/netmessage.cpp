#include "netmessage.h"

namespace qdg {
    
   std::ostream& operator<<(std::ostream& out, const NetMessage& message) {
       auto type = static_cast<int>(message.msg_type);
       auto p_id = static_cast<unsigned>(message.player_id);

       out << "[Player_id: " << p_id << "; ";
       out << "Type: " << type << "; ";

       switch (message.msg_type) {
           case NetMessage::Type::add_player:
               out << message.player_name;
               break;
            case NetMessage::Type::mark_position:
               out << message.position;
               break;
            case NetMessage::Type::set_die:
               out << message.die;
               break;
            default:
               break;
       }

       out << "]";
       return out;
   }

    sf::Packet& operator<<(sf::Packet& packet, const NetMessage::Type& msg_type) {
        auto t = static_cast<sf::Uint8>(msg_type);
        packet << t;

        return packet;
    }
    
    sf::Packet& operator>>(sf::Packet& packet, NetMessage::Type& msg_type) {
        sf::Uint8 t;
        packet >> t;
        msg_type = static_cast<NetMessage::Type>(t);

        return packet;
    }


    sf::Packet& operator<<(sf::Packet& packet, const NetMessage& message) {
        /* spdlog::info("Put player_id {} into packet in operator<<\n", message.player_id); */
        packet << message.player_id;
        /* spdlog::info("Put msg_type {} into packet\n", message.msg_type); */
        packet << message.msg_type;

        switch(message.msg_type) {
            case NetMessage::Type::add_player: {
                auto& player_name = message.player_name;
                /* spdlog::debug("Put player name {} into packet\n", player_name); */
                packet << player_name;
                break;
            }
            //don't need to do anything special for 
            //the following
            case NetMessage::Type::roll_dice:
            case NetMessage::Type::end_turn:
            case NetMessage::Type::take_penalty:
            case NetMessage::Type::host_ready:
            case NetMessage::Type::set_host:
            case NetMessage::Type::start_game:
                break;
            case NetMessage::Type::mark_position: {
                auto& position = message.position;
                /* spdlog::debug("Put position {} into packet\n", position); */
                packet << position;
                break;
            }
            case NetMessage::Type::set_die: {
                auto& die = message.die;
                /* spdlog::debug("Put die {} into packet\n", die); */
                packet << die;
                break;
            }
            default:
                /* spdlog::error("Unexpected message type in operator<<\n"); */
                break;
        }

        return packet;
    }

    sf::Packet& operator>>(sf::Packet& packet, NetMessage& message) {
        packet >> message.player_id;
        /* spdlog::debug("Got player_id {} from packet\n", message.player_id); */
        packet >> message.msg_type;
        /* spdlog::debug("Got msg_type {} from packet\n", message.msg_type); */

        switch(message.msg_type) {
            case NetMessage::Type::add_player: {
                std::string player_name;
                packet >> player_name;
                /* spdlog::debug("Got player name {} from packet\n", player_name); */
                message.player_name = player_name;
                break;
            }
            //don't need to do anything special for roll_dice or end_turn message
            case NetMessage::Type::set_host:
            case NetMessage::Type::host_ready:
            case NetMessage::Type::start_game:
            case NetMessage::Type::roll_dice:
            case NetMessage::Type::end_turn: 
            case NetMessage::Type::take_penalty: 
                break;
            case NetMessage::Type::mark_position: {
                Gameboard::Position position;
                packet >> position;
                /* spdlog::debug("Got position {} from packet\n", position); */
                message.position = position;
                break;
            }
            case NetMessage::Type::set_die: {
                Die die;
                packet >> die;
                /* spdlog::debug("Got die {} from packet\n", die); */
                message.die = die;
                break;
            }
            default:
                /* spdlog::error("Unexpected message type in operator>>\n"); */
                break;
        }

        return packet;
    }

}





