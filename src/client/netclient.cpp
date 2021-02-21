#include "netclient.h"

namespace qdg {

    void NetClient::set_non_blocking() {
        server.setBlocking(false);
    }

    void NetClient::set_server_ip(const std::string &ip) {
        server_ip = sf::IpAddress(ip);
    }

    void NetClient::set_server_port(const unsigned short port) {
        server_port = port;
    }

    bool NetClient::connect_to_server() { 
            spdlog::debug("Attempting to connect to server ...\n");
            auto status = server.connect(server_ip, server_port, sf::seconds(2.f));
            switch (status) {
                case sf::Socket::Disconnected:
                    spdlog::debug("Got disconnected\n");
                    break;
                case sf::Socket::Done:
                    spdlog::debug("Got done\n");
                    break;
                case sf::Socket::Partial:
                    spdlog::debug("Got partial\n");
                    break;
                case sf::Socket::Error:
                    spdlog::debug("Got error\n");
                    break;
                case sf::Socket::NotReady:
                    spdlog::debug("Got not ready\n");
                    break;
                default:
                    spdlog::debug("Not sure\n");
                    break;
            }

            if (status == sf::Socket::Done) {
                spdlog::debug("Successfully connected to server\n");
                return true;
            }

            spdlog::debug("Failed connecting to server\n");
            return false;
    }

    NetMessage NetClient::receive_message() {
        /* spdlog::debug("Checking for messages...\n"); */
        sf::Packet packet;
        NetMessage message;
        auto status = server.receive(packet);
        if (status == sf::Socket::Done) {
            packet >> message;
            return message;
        }

        /* else if (status == sf::Socket::NotReady) { */
        /*     spdlog::debug("No packet is ready\n"); */
        /* } */
        
        message.msg_type = NetMessage::Type::no_msg;
        
        return message;
    }

    void NetClient::set_id(sf::Uint8 id) {
        player_id = id;
    }

    void NetClient::set_host(bool h) {
        host = h;
    }

    bool NetClient::send_message() {
        if (packet_queue.size() == 0)
            return false;

        sf::Packet to_send = packet_queue.front();

        if (server.send(to_send) == sf::Socket::Done) {
            /* spdlog::debug("Fully sent packet, removing from queue\n"); */
            packet_queue.pop();
            return true;
        }

        else {
            /* spdlog::debug("Did not fully send packet, not removing from queue\n"); */
            return false;
        }
    }

    void NetClient::add_player(const std::string& name) {
        NetMessage message;
        message.player_id = player_id;
        message.msg_type = NetMessage::Type::add_player;
        message.player_name = name;

        sf::Packet packet;
        packet << message;

        packet_queue.push(packet);
    }

    void NetClient::set_ready() {
        if (!host) {
            spdlog::error("Only host can send host_ready message\n");
            return;
        }
        NetMessage message;
        message.player_id = player_id;
        message.msg_type = NetMessage::Type::host_ready;

        sf::Packet packet;
        packet << message;

        packet_queue.push(packet);
    }
    

    void NetClient::roll_dice() {
        NetMessage message;
        message.player_id = player_id;
        message.msg_type = NetMessage::Type::roll_dice;

        sf::Packet packet;
        packet << message;

        packet_queue.push(packet);
    }
        

    void NetClient::mark_position(const Gameboard::Position& position) {
        NetMessage message;
        message.player_id = player_id;
        message.msg_type = NetMessage::Type::mark_position;
        message.position = position;

        sf::Packet packet;
        packet << message;

        packet_queue.push(packet);
    }
        
    void NetClient::take_penalty() {
        NetMessage message;
        message.player_id = player_id;
        message.msg_type = NetMessage::Type::take_penalty;

        sf::Packet packet;
        packet << message;

        packet_queue.push(packet);
    }

    void NetClient::end_turn() {
        NetMessage message;
        message.player_id = player_id;
        message.msg_type = NetMessage::Type::end_turn;

        sf::Packet packet;
        packet << message;

        packet_queue.push(packet);
    }

    void NetClient::disconnect() {
        NetMessage message;
        message.player_id = player_id;
        message.msg_type = NetMessage::Type::disconnect;

        sf::Packet packet;
        packet << message;

        packet_queue.push(packet);
    }

    void NetClient::die_received(Die::Color color) {
        auto c = static_cast<unsigned>(color);

        received_die[c] = true;

        if (std::all_of(std::cbegin(received_die), std::cend(received_die),
                    [](bool b) { return b; })) {
            spdlog::debug("All dice have been received\n");
            all_die_received = true;
        }
    }


    bool NetClient::is_game_running() const noexcept {
        return game_running;
    }

    bool NetClient::is_host() const noexcept {
        return host;
    }

}
