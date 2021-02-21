#include "netserver.h"
#include <iostream>

/* TODO: Make it so that after server recieves start game message from */
/* host, any clients that are disconnected and did not send an add_player */
/* message are disconnected */

namespace qdg {
    NetServer::NetServer() {
        setup_listener();
    }

    void NetServer::prepare_game() {
        spdlog::debug("In prepare_game\n");
        selector_add_listener();
        while (!game_is_running) {
            //try to get message from clients
            if (selector.wait(wait_time)) {
                if (selector.isReady(listener)) {
                    get_new_client();
                }
                else {
                    process_messages();
                }
            }

        }
    }

    void NetServer::run_game() {
        selector_remove_listener();

        while (game_is_running) {
            //try to get message from clients
            if (selector.wait(wait_time)) {
                /* got_message = true; */
                process_messages();
            }
        }

        print_results();
    }

    void NetServer::print_results() {
        fmt::print("RESULTS\n------------------\n");
        fmt::print("{:10}\t\t{:5}\n", "Player", "Score");

        for (auto& [id, client] : clients) {
            auto& player = state.get_player(id);
            auto score = player.gameboard.calculate_score();
            fmt::print("{:10}\t\t{:5}\n", player.name, score);
        }
    }

    void NetServer::play_again() {
        state.reset_game();
        prepare_game();
        run_game();
    }

    void NetServer::clear_clients() {
        for (auto& [id, client] : clients) {
            client->disconnect();
            state.remove_player(sf::Uint8(id));
        }

        clients.clear();
        selector.clear();
    }
            

    void NetServer::get_new_client() {
        auto client = std::make_unique<sf::TcpSocket>();
        if (listener.accept(*client) != sf::Socket::Done) {
            spdlog::error("Error accepting client, exiting\n");
            exit(1);
        }

        sf::Uint8 id = 0;

        if (!free_ids.empty()) {
            id = free_ids.front();
            free_ids.pop();
        }

        else {
            id = clients.size();
        }

        spdlog::debug("Accepted a new client\n");
        clients.insert({id, std::move(client)});
        selector.add(*clients[id]);
        
/* TODO: Maybe there should be some setting about who can be host? */
        
        // send the client back their id and inform them if they
        // are the host or not
        NetMessage message;
        message.player_id = id;
        if (clients.size() == 1) {
            host_id = id;
            message.msg_type = NetMessage::Type::set_host;
        }

        else {
            message.msg_type = NetMessage::Type::not_host;
        }

        //the player_id is the clients id
        message_client(message.player_id, message);
    }

    void NetServer::process_messages() {
        NetMessage message;

        for (auto& [id, client] : clients) {
            if (selector.isReady(*client)) {
                spdlog::debug("Client {} is ready to send\n", id);
                message = receive_message(*client);
                if (message.msg_type != NetMessage::Type::no_msg) {
                    handle_message(id, message);
                }
            }
        }
    }

    NetMessage NetServer::receive_message(sf::TcpSocket& client) {
        sf::Packet packet;
        NetMessage message;
        auto status = client.receive(packet);

        if(status == sf::Socket::Done) {
            packet >> message;
            return message;
        }

        else if (status == sf::Socket::Disconnected) {
            spdlog::debug("The client is disconnected\n");
            client.disconnect();
            selector.remove(client);

        }

        else {
            spdlog::error("Server did not fully receive packet\n");
        }

        message.msg_type = NetMessage::Type::no_msg;
        return message;
    }

    void NetServer::handle_message(sf::Uint8 id, NetMessage& message) {
        spdlog::debug("Got message {}\n", message);
        if (message.player_id != id) {
            spdlog::error("Message {} does not have id {}\n", message, id);
        }
        switch(message.msg_type) {
            // we are asumming that the client already has the correct id
            case NetMessage::Type::add_player: {
                state.add_player(message.player_id, message.player_name);
                message_all_clients(message);
                break;
            }

            case NetMessage::Type::host_ready: 
                //id 0 is the host
                if (id == host_id) {
                    spdlog::debug("Host is ready\n");
                    game_is_running = true;
                    message.msg_type = NetMessage::Type::start_game;
                    state.init_leader();
                    message.player_id = state.get_leader();
                    message_all_clients(message);
                    break;
                }
                spdlog::error("Someone other than host sent host_ready msg\n");
                break;

            case NetMessage::Type::roll_dice:
                message_all_clients(message);
                state.roll_dice();
                send_dice_configuration();
                break;
            case NetMessage::Type::mark_position:
                state.mark_position(message.player_id, message.position);
                message_all_clients(message);
                break;
            case NetMessage::Type::take_penalty:
                state.take_penalty(message.player_id);
                message_all_clients(message);
                break;
            case NetMessage::Type::end_turn:
                //gamestate::end_turn will return true if we are ready
                //for next round, but we need to check if the game is over before we proceed
                //in that case send new round message, with new leader id
                if (state.end_turn(message.player_id)) {
                    if (!check_game_over()) {
                    message.msg_type = NetMessage::Type::new_round;
                    message.player_id = state.get_leader();
                    message_all_clients(message);
                    }
                }
                break;
            case NetMessage::Type::disconnect:
                spdlog::debug("Disconnecting client {}\n", id);
                //remove client from state, disconnect, 
                //remove from clients, and add id to free_ids
                state.remove_player(sf::Uint8(id));
                clients[id]->disconnect();
                clients.erase(id);
                free_ids.push(id);
                message_all_clients(message);

                if (clients.size() == 0) {
                    spdlog::debug("All clients removed\n");
                }

                if (id == host_id) {
                    //need to get a new host
                    host_id = clients.begin()->first;
                    message.msg_type = NetMessage::Type::set_host;
                    message_client(host_id, message);
                }

                break;
            default:
                spdlog::debug("Unexpected message in handle_message\n");
                break;
        }
    }

    bool NetServer::check_game_over() {
        spdlog::debug("Checking if game is over...\n");
        if (state.check_game_over()) {
            spdlog::debug("Game is over, notifying clients\n");
            NetMessage message;
            message.msg_type = NetMessage::Type::game_over;
            message_all_clients(message);
            game_is_running = false;
            return true;
        }
        return false;
    }

    bool NetServer::any_connected() const noexcept {
        return clients.size() > 0;
    }

    void NetServer::message_client(sf::Uint8 id, const NetMessage& message) {

        sf::Packet packet;
        packet << message;

        spdlog::debug("Sent message {} to client {}\n", message, id);
        clients[id]->send(packet);
    }

    void NetServer::message_all_clients(const NetMessage& message) {
        for (auto& [id, client] : clients) {
            message_client(id, message);
        }
    }
      
    void NetServer::send_dice_configuration() {
        spdlog::debug("Sending dice configuration");

        for (auto& color : all_die_colors) {
            NetMessage message;
            auto die = state.get_die(color);
            message.msg_type = NetMessage::Type::set_die;
            message.die = die;
            message_all_clients(message);
        }
    }
    
    void NetServer::setup_listener() {
        spdlog::debug("Sever is listening on port {}\n", port);
        if (listener.listen(port) != sf::Socket::Done) {
            spdlog::error("Error listening, exiting\n");
            exit(1);
        }
    }

    void NetServer::selector_add_listener() {
        selector.add(listener);
    }

    void NetServer::selector_remove_listener() {
        selector.remove(listener);
    }
}

