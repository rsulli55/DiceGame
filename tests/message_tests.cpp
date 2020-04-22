#include <catch2/catch.hpp>
#include <spdlog/common.h>
#include "../src/netmessage.h"

TEST_CASE("Setting up a listener and sending messages", "[networking]") {

    spdlog::set_level(spdlog::level::debug);

    std::array<unsigned short, 5> ports = {
        (unsigned short) 54'000, (unsigned short) 54'001, 
        (unsigned short) 54'002, (unsigned short) 54'003, 
        (unsigned short) 54'004 
    };
    const auto server_ip = sf::IpAddress("192.168.0.249");
    sf::TcpListener listener;
    
    SECTION("Send player add message") {
        if (listener.listen(ports[0]) != sf::Socket::Done) {
            spdlog::error("Failed setting up listener, exiting\n");
            exit(1);
        }
        
        sf::TcpSocket socket;
        sf::Socket::Status status = socket.connect(server_ip, ports[0]);
        if (status != sf::Socket::Done) {
            spdlog::error("Failed setting up socket, exiting\n");
            exit(1);
        }


        spdlog::debug("Trying to accept a client\n");
        sf::TcpSocket client;
        if (listener.accept(client) != sf::Socket::Done) {
            spdlog::error("Could not accept client, exiting\n");
            exit(1);
        }

        qdg::NetMessage sent_message;
        sent_message.player_id = 0;
        sent_message.msg_type = qdg::NetMessage::Type::add_player;
        sent_message.player_name = "Player0";

        sf::Packet sent_packet;
        sent_packet << sent_message;

        socket.send(sent_packet);

        sf::Packet rec_packet;

        client.receive(rec_packet);

        qdg::NetMessage rec_message;
        rec_packet >> rec_message;

        REQUIRE(rec_message.player_id == sent_message.player_id);
        REQUIRE(rec_message.msg_type == sent_message.msg_type);
        REQUIRE(rec_message.player_name == sent_message.player_name);

        sf::Packet server_to_client;
        server_to_client << rec_message;
        spdlog::debug("Sending message from server to client\n");
        client.send(server_to_client);
        sf::Packet client_from_server;
        socket.receive(client_from_server);
        qdg::NetMessage message_from_server;
        client_from_server >> message_from_server;

        REQUIRE(rec_message.player_id == message_from_server.player_id);
        REQUIRE(rec_message.msg_type == message_from_server.msg_type);
        REQUIRE(rec_message.player_name == message_from_server.player_name);
    }

    SECTION("Send roll dice message") {
        if (listener.listen(ports[1]) != sf::Socket::Done) {
            spdlog::error("Failed setting up listener, exiting\n");
            exit(1);
        }
        
        sf::TcpSocket socket;
        sf::Socket::Status status = socket.connect(server_ip, ports[1]);
        if (status != sf::Socket::Done) {
            spdlog::error("Failed setting up socket, exiting\n");
            exit(1);
        }


        spdlog::debug("Trying to accept a client\n");
        sf::TcpSocket client;
        if (listener.accept(client) != sf::Socket::Done) {
            spdlog::error("Could not accept client, exiting\n");
            exit(1);
        }
        qdg::NetMessage sent_message;
        sent_message.player_id = 0;
        sent_message.msg_type = qdg::NetMessage::Type::roll_dice;

        sf::Packet sent_packet;
        sent_packet << sent_message;

        socket.send(sent_packet);

        sf::Packet rec_packet;

        client.receive(rec_packet);

        qdg::NetMessage rec_message;
        rec_packet >> rec_message;

        REQUIRE(rec_message.player_id == sent_message.player_id);
        REQUIRE(rec_message.msg_type == sent_message.msg_type);
    }

    SECTION("Send end turn message") {
        if (listener.listen(ports[2]) != sf::Socket::Done) {
            spdlog::error("Failed setting up listener, exiting\n");
            exit(1);
        }
        
        sf::TcpSocket socket;
        sf::Socket::Status status = socket.connect(server_ip, ports[2]);
        if (status != sf::Socket::Done) {
            spdlog::error("Failed setting up socket, exiting\n");
            exit(1);
        }


        spdlog::debug("Trying to accept a client\n");
        sf::TcpSocket client;
        if (listener.accept(client) != sf::Socket::Done) {
            spdlog::error("Could not accept client, exiting\n");
            exit(1);
        }

        qdg::NetMessage sent_message;
        sent_message.player_id = 0;
        sent_message.msg_type = qdg::NetMessage::Type::end_turn;

        sf::Packet sent_packet;
        sent_packet << sent_message;

        socket.send(sent_packet);

        sf::Packet rec_packet;

        client.receive(rec_packet);

        qdg::NetMessage rec_message;
        rec_packet >> rec_message;

        REQUIRE(rec_message.player_id == sent_message.player_id);
        REQUIRE(rec_message.msg_type == sent_message.msg_type);

    }

    SECTION("Send mark position message") {
       
        if (listener.listen(ports[3]) != sf::Socket::Done) {
            spdlog::error("Failed setting up listener, exiting\n");
            exit(1);
        }
        
        sf::TcpSocket socket;
        sf::Socket::Status status = socket.connect(server_ip, ports[3]);
        if (status != sf::Socket::Done) {
            spdlog::error("Failed setting up socket, exiting\n");
            exit(1);
        }


        spdlog::debug("Trying to accept a client\n");
        sf::TcpSocket client;
        if (listener.accept(client) != sf::Socket::Done) {
            spdlog::error("Could not accept client, exiting\n");
            exit(1);
        }

        qdg::NetMessage sent_message;
        sent_message.player_id = 0;
        sent_message.msg_type = qdg::NetMessage::Type::mark_position;
        auto pos = qdg::Gameboard::Position { 
            qdg::Gameboard::Color::red, 
            qdg::Gameboard::Number::two
        };

        sent_message.position = pos;

        sf::Packet sent_packet;
        sent_packet << sent_message;

        socket.send(sent_packet);

        sf::Packet rec_packet;

        client.receive(rec_packet);

        qdg::NetMessage rec_message;
        rec_packet >> rec_message;

        REQUIRE(rec_message.player_id == sent_message.player_id);
        REQUIRE(rec_message.msg_type == sent_message.msg_type);
        REQUIRE(rec_message.position == sent_message.position);
    }
}






