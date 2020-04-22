#pragma once

#include <queue>
#include <SFML/Network/TcpSocket.hpp>
#include "../gamestate.h"
#include "../netmessage.h"

namespace qdg {

    /// Manages the network communications from the client side
    class NetClient {
        public:
            NetClient();
            bool connect_to_server();
            void set_non_blocking();
            NetMessage receive_message();

            void set_server_ip(const std::string& ip);
            void set_server_port(const short unsigned port);

            void set_id(sf::Uint8 id);
            void set_host(bool h);
            /* void handle_message(NetMessage& message); */
            NetMessage process_message();
            /// \effects attempts to send the first message in `packet_queue`
            bool send_message();
            /// \effects adds `add_player` message using name to `packet_queue`  
            void add_player(const std::string& name);
            /// \effects adds `host_ready` message to `packet_queue`  
            /// \requires host is true
            void set_ready();
            /// \effects adds `roll_dice` message to `packet_queue`  
            void roll_dice();
            /// \effects adds `mark_position` message using position to `packet_queue`  
            void mark_position(const Gameboard::Position& position);
            /// \effects adds `take_penalty` message to `packet_queue`  
            void take_penalty();
            /// \effects adds `end_turn` message to `packet_queue`  
            void end_turn();
            void disconnect();

            bool is_game_running() const noexcept;
            bool is_host() const noexcept;

        private:
            /// \effects marks the corresponding position
            /// in received_die to true and if all die colors
            /// have been received sets all_die_received to true
            void die_received(Die::Color color);
            Gamestate state;
            sf::TcpSocket server;
            sf::Uint8 player_id;
            bool host = false;
            bool game_running = false;
            bool dice_rolling = false;
            std::queue<sf::Packet> packet_queue;
            std::array<bool, number_of_dice> received_die {
                false, false, false,
                false, false, false
            };
            bool all_die_received = false;

            
            /* const sf::IpAddress server_ip = sf::IpAddress(1186864313); */
            sf::IpAddress server_ip = sf::IpAddress("192.168.0.249");
            unsigned short server_port = (unsigned short) 54'200;
    };
}
