#include "gamestate.h"

namespace qdg{ 

    /* void Gamestate::start_game() { */
    /*     spdlog::debug("Starting game\n"); */
    /*     reset_game(); */
    /* } */

    void Gamestate::reset_game() {
        spdlog::debug("Resetting game\n");

        for (auto& [id, player] : players) {
            player.gameboard.reset();
            player.penalties = 0;
        }

        std::fill(std::begin(locked_colors), std::end(locked_colors), false);

        /* leader = players.begin(); */
    }

    void Gamestate::add_player(sf::Uint8 id, const std::string& player_name) {
        players.insert({id, {player_name}});
    }

    void Gamestate::remove_player(sf::Uint8 id) {
        //move to next player for leader
        //actually don't worry about leader
        /* leader++; */
        players.erase(id);
        //reset leader iterator
        /* leader = players.find(leader->first); */
    }

    Player& Gamestate::get_player(sf::Uint8 player_id) noexcept {
        //this is not really noexcept at the moment because of
        //the players[0] part, but the plan is 
        //to remove the check anyway 
        //(I don't see how I will send an invalid id here, but for testing
        //I've included the check)
        if (player_id >= players.size()) {
            spdlog::error("Gamestate::player got an invalid player id\n");
            spdlog::error("Got id {}, but only have {} players\n", player_id, players.size());
            return players[0];
        }
        return players[player_id];
    }

    void Gamestate::set_leader(sf::Uint8 id) {
        leader_id = id;
    }

    sf::Uint8 Gamestate::get_leader() {
        return leader_id;
    }

    void Gamestate::lock_color(const Gameboard::Color& color) noexcept {
        auto c = static_cast<int>(color);

        locked_colors[c] = true;
    }

    /* void Gamestate::next_round()  noexcept { */
    /*     spdlog::debug("Starting next round\n"); */
        
    /*     leader++; */
    /*     if (leader == players.end()) */
    /*         leader = players.begin(); */
    /* } */
    
    void Gamestate::set_die(Die die) noexcept {
        auto c = static_cast<unsigned>(die.color);

        dice[c] = die;

    }

    void Gamestate::mark_position(sf::Uint8 player_id, Gameboard::Position position) noexcept {
        if (player_id >= players.size()) {
            spdlog::error("Gamestate::mark_position got an invalid player id\n");
            spdlog::error("Got id {}, but only have {} players\n", player_id, players.size());
            return;
        }
        auto& player = players[player_id];
        bool locked = player.gameboard.mark_position(position);

        if (locked) {
            auto c = static_cast<unsigned>(position.color);
            locked_colors[c] = true;
        }

    }

    void Gamestate::take_penalty(sf::Uint8 player_id) noexcept {
        if (player_id >= players.size()) {
            spdlog::error("Gamestate::take_penalty got an invalid player id\n");
            spdlog::error("Got id {}, but only have {} players\n", player_id, players.size());
            return;
        }

        players[player_id].penalties++;
    }

}
