#include <catch2/catch.hpp>
#include "../src/server/netserver.h"
#include "../src/client/netclient.h"

/* NOTE: This test assumes that the gameserver is already runnning */
/* For that reason, it is not automatically included with the `tests` target */

TEST_CASE("Test a run of the game", "[gameserver]") {
    sf::Time my_countdown = sf::seconds(1.f);
    sf::Clock my_clock;
    sf::Time my_elapsed = sf::Time::Zero;
    spdlog::set_level(spdlog::level::debug);

    std::vector<std::unique_ptr<qdg::NetClient>> clients;
    int num_clients = 4;

    for (int i = 0; i < num_clients; ++i) {
        auto client = std::make_unique<qdg::NetClient>();
        clients.push_back(std::move(client));
    }

    //game preparation
    std::queue<qdg::NetClient*> to_connect;
    int client_count = 0;
    for (auto& client : clients) {
        client_count++;
        to_connect.push(client.get());
    }

    int attempts = 0;
    while (!to_connect.empty() && attempts < 8) {
        auto client = to_connect.front();
        attempts++;
        spdlog::error("Client count {}\n", client_count);

        if (client->connect_to_server()) {
            spdlog::debug("Successfully connected\n");
            to_connect.pop();
        }

        else {
            spdlog::debug("Did not connect, retrying\n");
        }
    }

    if (attempts == 8)
        exit(1);

    //clients[0] should be the host
    auto& host = clients[0];
    bool players_sent = false;
    bool set_ready = false;

    while (!host->is_game_running()) {
        for (auto& client : clients) {
            my_elapsed += my_clock.restart();

            if (!players_sent && my_elapsed >= my_countdown) {
                for (int i = 0; i < num_clients; ++i) {
                    auto& client = clients[i];
                    auto name = fmt::format("Player {}", i);
                    client->add_player(name);
                }
                players_sent = true;
            }

            else if (!set_ready && my_elapsed >= 2.f * my_countdown) {
                host->set_ready();
                set_ready = true;
            }

            spdlog::error("HERE112341\n");
            client->process_message();
            client->send_message();
        }
    }

    REQUIRE(host->is_host() == true);

    SECTION("Run of game where game ends because of two many penalties") {
        host->take_penalty();
        host->take_penalty();
        host->take_penalty();
        host->take_penalty();

        while(host->is_game_running()) {
            for (auto& client : clients) {
                spdlog::debug("HERE\n");
                client->process_message();
                client->send_message();
            }
        }
    }

    SECTION("Run of game where game after two colors are locked") {
        host->mark_position({qdg::Gameboard::Color::red, qdg::Gameboard::Number::two});
        host->mark_position({qdg::Gameboard::Color::red, qdg::Gameboard::Number::five});
        host->mark_position({qdg::Gameboard::Color::red, qdg::Gameboard::Number::six});
        host->mark_position({qdg::Gameboard::Color::red, qdg::Gameboard::Number::seven});
        host->mark_position({qdg::Gameboard::Color::red, qdg::Gameboard::Number::eight});
        host->mark_position({qdg::Gameboard::Color::red, qdg::Gameboard::Number::twelve});
        host->mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::two});
        host->mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::five});
        host->mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::six});
        host->mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::seven});
        host->mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::eight});
        host->mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::twelve});

        while(host->is_game_running()) {
            for (auto& client : clients) {
                client->process_message();
                client->send_message();
            }
        }

        for (auto& client : clients) {
            client->disconnect();
        }

        std::queue<unsigned> not_sent;
        for (int i = 0; i < num_clients; ++i) {
            not_sent.push(i);
        }

        while (!not_sent.empty()) {
            auto id = not_sent.front();
            if (clients[id]->send_message()) {
                not_sent.pop();
            }
        }

    }

}

