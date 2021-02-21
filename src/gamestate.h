#pragma once

#include <array>
#include <vector>
#include <map>
#include <deque>
#include <algorithm>
#include <SFML/Config.hpp>

#include "utility.h"
#include "player.h"
#include "die.h"

namespace qdg {
    /// Class that manages a clients gamestate
    ///   
    /// Holds a vector Players (make a link) each of which has a Gameboard (make a link). 
    /// The `deque<bool>` `turn_done` stores which players have submitted the `end_turn` message.
    /// Also has a array of 6 die to represent the various dice from the game
    /// Stores the id of the current leader/roller.
    class Gamestate {
        public:
        
            /// \effects calls resets each players gameboard
            void reset_game();
            /// \effects makes a new Player with the name and adds it to 
            /// the players map
            void add_player(sf::Uint8 id, const std::string& player_name);
            void remove_player(sf::Uint8 id);
            Player& get_player(sf::Uint8 player_id) noexcept;
            void set_leader(sf::Uint8 id);
            sf::Uint8 get_leader();
            /// \effects Sets each die to a new Die::Face by calling roll_one_die()
            /// \effects calls Gameboard::mark_position for the Gameboard owned by player_id
            /// if Gameboard::mark_position returns true, we lock the Gameboard::Color associated
            /// with position
            void mark_position(sf::Uint8 player_id, Gameboard::Position) noexcept;
            /// \effects Increases the penalty count of Player with id player_id
            void take_penalty(sf::Uint8 player_id) noexcept;
            void set_die(Die die) noexcept;
/* TODO: can lock_color be made private? */
            void lock_color(const Gameboard::Color& color) noexcept;

        protected:
/* TODO: Is there a reason why dice is an array of die instead of just an */ 
/*           array of Die::Face? */
            std::array<Die, number_of_dice> dice {
                Die{Die::Color::white1, Die::Face::one, true},
                Die{Die::Color::white2, Die::Face::one, true},
                Die{Die::Color::red, Die::Face::one, true},
                Die{Die::Color::yellow, Die::Face::one, true},
                Die{Die::Color::green, Die::Face::one, true},
                Die{Die::Color::blue, Die::Face::one, true} 
            };

            std::map<sf::Uint8, Player> players;
            sf::Uint8 leader_id;
            std::array<bool, game_num_colors> locked_colors{false, false, false,
                                                          false};
    };
}
