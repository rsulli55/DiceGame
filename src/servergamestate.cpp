#include "servergamestate.h"

namespace qdg { 
    void ServerGamestate::add_player(sf::Uint8 id, const std::string& player_name) {
        Gamestate::add_player(id, player_name);
        turn_done.insert({id, false});
    }

    void ServerGamestate::remove_player(sf::Uint8 id) {
/* TODO: Figure out how to handle removing the leader */
        Gamestate::remove_player(id);
        turn_done.erase(id);
        leader = players.find(leader_id);
    }

    void ServerGamestate::init_leader() noexcept {
        leader = players.begin();
        leader_id = leader->first;

        spdlog::debug("The players map starting at leader is\n");
        for (auto it = leader; it != players.end(); ++it) {
            spdlog::debug("Id: {}, name: {}", it->first, it->second.name);
        }
    }

    void ServerGamestate::roll_dice() noexcept {
        for (auto& die : dice) {
            auto f = roll_one_die();
            auto face = static_cast<Die::Face>(f - 1);
            die.face = face;
        }
    }

    Die::Face ServerGamestate::get_face(Die::Color color) const noexcept {
        auto c = static_cast<unsigned>(color);

        return dice[c].face;
    }

    Die ServerGamestate::get_die(Die::Color color) const noexcept {
        auto c = static_cast<unsigned>(color);

        return dice[c];
    }

    bool ServerGamestate::end_turn(sf::Uint8 player_id) {
        if (player_id >= players.size()) {
            spdlog::error("Gamestate::end_turn got an invalid player id\n");
            spdlog::error("Got id {}, but only have {} players\n", player_id, players.size());
            return false;
        }

        turn_done[player_id] = true;

        for (auto& [id, done] : turn_done) {
            if (!done)
                return false;
        }

        new_round(); 
        return true;
    }
    
    void ServerGamestate::new_round()  noexcept {
        //reset turn_done vector
        for (auto& [id, done] : turn_done) {
            done = false;
        }
        //update leader
        leader++;
        spdlog::debug("Incremented leader\n");
        if (leader == players.end()) {
            spdlog::debug("Leader if players.end(), so resetting it\n");
            leader = players.begin();
        }
        spdlog::debug("New leader id: {}, name {}\n", leader->first, leader->second.name);
        leader_id = leader->first;

    }

    bool ServerGamestate::check_game_over() const noexcept {
        auto locked = std::count(std::cbegin(locked_colors), 
                std::cend(locked_colors), true);

        if (locked >= locked_needed_to_end)
            return true;

        for (const auto& [id, p] : players) {
            if (p.penalties >= penalties_needed_to_end)
                return true;
        }

        return false;
    }

}
