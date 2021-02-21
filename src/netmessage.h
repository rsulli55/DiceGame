#pragma once

#include <spdlog/spdlog.h>
#include <string>
#include <SFML/Network.hpp>

#include "gameboard.h"
#include "die.h"

/* Currently all Messages are under this one class, maybe having multiple 
 * classes would be better?*/

namespace qdg {

    /// A messaging wrapper class
    struct NetMessage {

        enum class Type { 
            no_msg,
            /// Initial setup messages
            set_host, not_host,
            add_player, 
            host_ready, start_game, 
            /// Gameplay messages
            new_round, 
            roll_dice, set_die,
            mark_position, take_penalty, 
            end_turn, game_over, 
            /// After game messages
            disconnect, restart_game
        };

        sf::Uint8 player_id;
        std::string player_name;
        Type msg_type;
        Gameboard::Position position;
        Die die;

    };
        
    std::ostream& operator<<(std::ostream& out, const NetMessage& message);
    sf::Packet& operator<<(sf::Packet& packet, const NetMessage::Type& msg_type);
    sf::Packet& operator>>(sf::Packet& packet, NetMessage::Type& msg_type);
    sf::Packet& operator<<(sf::Packet& packet, const NetMessage& message);
    sf::Packet& operator>>(sf::Packet& packet, NetMessage& message);
}
