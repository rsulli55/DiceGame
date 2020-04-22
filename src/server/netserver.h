#pragma once

#include <queue>
#include <memory>
#include <SFML/Network/TcpSocket.hpp>
#include "../servergamestate.h"
#include "../netmessage.h"

namespace qdg {
    /// Managers network communications from the server side
    class NetServer {
        public:
            NetServer();
            void prepare_game();
            void run_game();
            void play_again();
            void clear_clients();
            /// \notes when receiving a host_ready msg from the host
            /// sets `game_is_running` to `true` and sends `start_game` 
            /// message to all clients
            void handle_message(sf::Uint8 id, NetMessage& message);
            /// \effects sets `game_is_running` to `false` if the game has ended
            /// and sends `game_over` msg to all clients
            bool check_game_over();
            bool any_connected() const noexcept;
        private:
            void setup_listener();
            void selector_add_listener();
            void selector_remove_listener();
            void message_client(sf::Uint8 id, const NetMessage& message);
            void message_all_clients(const NetMessage& message);
            NetMessage receive_message(sf::TcpSocket& client);
            void get_new_client();
            void process_messages();
            void send_dice_configuration();
            void print_results();
            ServerGamestate state;
            bool game_is_running = false;
            sf::SocketSelector selector;
            sf::TcpListener listener;
            std::map<sf::Uint8, std::unique_ptr<sf::TcpSocket>> clients;
            sf::Uint8 host_id;
            std::queue<sf::Uint8> free_ids;

            const unsigned short port = (unsigned short) 54'005;
/* TODO: Is this a good value for wait_time? */
            const sf::Time wait_time = sf::seconds(2.f);
    };
}


