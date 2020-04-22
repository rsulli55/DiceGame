#include "netclient.h"
#include <iostream>

int main () {
    spdlog::set_level(spdlog::level::debug);

    qdg::NetClient client;

    char wait_for = 'y';

    while (!client.connect_to_server() && wait_for == 'y') {
        fmt::print("did not connect, retry?\n");
        std::cin >> wait_for;
    }

    client.process_message();


    client.set_non_blocking();

    fmt::print("Send player name? ");
    std::cin >> wait_for;
    client.add_player("Player0");

    int attempts = 0;
    while (wait_for == 'y') {
        attempts++;
        client.process_message();
        client.send_message();
        if (attempts >= 5) {
            fmt::print("Attempted 5 times continue ? ");
            std::cin >> wait_for;
            attempts = 0;
        }
    }

    fmt::print("Set host_ready? ");
    std::cin >> wait_for;
    client.set_ready();

    attempts = 0;
    while (wait_for == 'y') {
        attempts++;
        client.process_message();
        client.send_message();
        if (attempts >= 5) {
            fmt::print("Attempted 5 times continue ? ");
            std::cin >> wait_for;
            attempts = 0;
        }
    }
        
    
    /* sf::Time my_countdown = sf::seconds(1.f); */
    /* sf::Clock my_clock; */
    /* sf::Time my_elapsed = sf::Time::Zero; */
    /* spdlog::set_level(spdlog::level::debug); */

    /* std::vector<std::unique_ptr<qdg::NetClient>> clients; */
    /* int num_clients = 4; */

    /* for (int i = 0; i < num_clients; ++i) { */
    /*     auto client = std::make_unique<qdg::NetClient>(); */
    /*     clients.push_back(std::move(client)); */
    /* } */

    /* auto host = clients[0].get(); */

    /* for (auto& client :clients) { */
    /*     while (!client->connect_to_server()) { */
    /*         spdlog::debug("Did not connect, retrying\n"); */
    /*     } */
    /* } */
    /* int attempts = 0; */
    /* std::queue<qdg::NetClient*> to_connect; */
    /* int client_count = 0; */
    /* for (auto& client : clients) { */
    /*     client_count++; */
    /*     to_connect.push(client.get()); */
    /* } */

    /* while (!to_connect.empty() && attempts < 30) { */
    /*     auto client = to_connect.front(); */
    /*     attempts++; */
    /*     spdlog::error("Client count {}\n", client_count); */

    /*     if (client->connect_to_server()) { */
    /*         spdlog::debug("Successfully connected\n"); */
    /*         to_connect.pop(); */
    /*     } */

    /*     else { */
    /*         spdlog::debug("Did not connect, retrying\n"); */
    /*     } */
    /* } */

    /* if (attempts == 30) */
    /*     exit(1); */

    /* char wait_for; */
    /* fmt::print("Send player names?  "); */
    /* std::cin >> wait_for; */
    /* for (int i = 0; i < num_clients; ++i) { */
    /*     auto& client = clients[i]; */
    /*     auto name = fmt::format("Player {}", i); */
    /*     client->add_player(name); */
    /* } */

    /* wait_for = 'y'; */
    /* attempts = 0; */

    /* while (wait_for) { */
    /*     for (auto& client : clients) { */
    /*         attempts++; */
    /*         client->process_message(); */
    /*         client->send_message(); */
    /*     } */

    /*     if (attempts >= 5) { */
    /*         fmt::print("Attempted {} times to send/receive, continue?  ", attempts); */
    /*         std::cin >> wait_for; */
    /*     } */
    /* } */


    /* fmt::print("Host ready?  "); */
    /* std::cin >> wait_for; */
    /* host->set_ready(); */

    /* wait_for = 'y'; */
    /* attempts = 0; */

    /* while (wait_for) { */
    /*     for (auto& client : clients) { */
    /*         attempts++; */
    /*         client->process_message(); */
    /*         client->send_message(); */
    /*     } */

    /*     if (attempts >= 5) { */
    /*         fmt::print("Attempted {} times to send/receive, continue?  ", attempts); */
    /*         std::cin >> wait_for; */
    /*     } */
    /* } */

    //game preparation

    //clients[0] should be the host
    /* auto& host = clients[0]; */
    /* bool players_sent = false; */
    /* bool set_ready = false; */

    /* while (!host->is_game_running()) { */
    /*     for (auto& client : clients) { */ /*         my_elapsed += my_clock.restart(); */

    /*         if (!players_sent && my_elapsed >= my_countdown) { */
    /*             for (int i = 0; i < num_clients; ++i) { */
    /*                 auto& client = clients[i]; */
    /*                 auto name = fmt::format("Player {}", i); */
    /*                 client->add_player(name); */
    /*             } */
    /*             players_sent = true; */
    /*         } */

    /*         else if (!set_ready && my_elapsed >= 2.f * my_countdown) { */
    /*             host->set_ready(); */
    /*             set_ready = true; */
    /*         } */
    /*         client->process_message(); */
    /*         client->send_message(); */
    /*     } */
    /* } */
    
    /* host->take_penalty(); */
    /* host->take_penalty(); */
    /* host->take_penalty(); */
    /* host->take_penalty(); */

    /* while(host->is_game_running()) { */
    /*     for (auto& client : clients) { */
    /*         spdlog::debug("HERE!!\n"); */
    /*         client->process_message(); */
    /*         client->send_message(); */
    /*     } */
    /* } */

    /* fmt::print("Ready for next game\n"); */
    /* std::string wait_for_key; */
    /* std::cin >> wait_for_key; */

    /* host->set_ready(); */
    /* while (!host->is_game_running()) { */
    /*     for (auto& client : clients) { */
    /*         client->process_message(); */
    /*         client->send_message(); */
    /*     } */
    /* } */
    /* host->mark_position({qdg::Gameboard::Color::red, qdg::Gameboard::Number::two}); */
    /* host->mark_position({qdg::Gameboard::Color::red, qdg::Gameboard::Number::five}); */
    /* host->mark_position({qdg::Gameboard::Color::red, qdg::Gameboard::Number::six}); */
    /* host->mark_position({qdg::Gameboard::Color::red, qdg::Gameboard::Number::seven}); */
    /* host->mark_position({qdg::Gameboard::Color::red, qdg::Gameboard::Number::eight}); */
    /* host->mark_position({qdg::Gameboard::Color::red, qdg::Gameboard::Number::twelve}); */
    /* host->mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::two}); */
    /* host->mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::five}); */
    /* host->mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::six}); */
    /* host->mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::seven}); */
    /* host->mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::eight}); */
    /* host->mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::twelve}); */

    /* while(host->is_game_running()) { */
    /*     for (auto& client : clients) { */
    /*         client->process_message(); */
    /*         client->send_message(); */
    /*     } */
    /* } */
}
