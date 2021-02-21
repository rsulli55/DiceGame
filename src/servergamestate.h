#pragma once

#include "gamestate.h"


namespace qdg {

    /// Class that manages the server's gamestate
    ///   
    /// Holds a vector of Players (make a link) each of which has a Gameboard (make a link). 
    /// The `map<sf::Uint8, bool>` `turn_done` stores which players have submitted the `end_turn` message.
    /// Also has a array of 6 die to represent the various dice from the game.
    /// Stores the id of the current leader/roller.
    class ServerGamestate : public Gamestate {
        public:
            /// \effects makes a new Player with the name and adds it to 
            /// the players map and sets `turn_done` to false for this `id`
            void add_player(sf::Uint8 id, const std::string& player_name);
            /// \notes Need to figure out how to handle if `id` is `leader_id`
            /// currently it will probably crash 
            void remove_player(sf::Uint8 id);
            void init_leader() noexcept;
            /// \effects Sets each die to a new Die::Face by calling roll_one_die()
            void roll_dice() noexcept;
/* TODO: Is get_face() needed? */
            Die::Face get_face(Die::Color color) const noexcept;
            Die get_die(Die::Color color) const noexcept;
            /// \effects marks turn_done[player_id] as true
            /// if every player has ended their turn, calls /// [next_round()](standardese://*next_round/)
            /// \returns true if every player has ended their turn, else false
            bool end_turn(sf::Uint8 player_id);
            /// \effects Sets the game leader to the next player id
            /// resets the end_turn deque
            void new_round() noexcept;
            bool check_game_over() const noexcept;

        private:
            std::map<sf::Uint8, Player>::iterator leader;
            std::map<sf::Uint8, bool> turn_done;

    };
}
