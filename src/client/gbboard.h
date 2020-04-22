#pragma once

#include <SFML/Graphics.hpp>

#include "../player.h"
#include "gbbox.h"


namespace qdg {

    ///Class that manages the graphics of the gameboard
    class GbBoard {
        public:

            GbBoard();
            //==modifiers==//
            /// \effects clear out any marks on gameboard
            /// \group modifiers
            void reset(); 

            /// \effects marks position on the gameboard
            /// \returns  true if the move locked the color otherwise false
            /// \group modifiers
            bool mark_position(const Gameboard::Position& position) noexcept;
            
            sf::Uint32 calculate_score() const; 
           
            /// \returns true if the position is marked on the gameboard
            /* bool check_position(const Position& position) const noexcept; */

            /// \returns true if marking position is a valid play
            bool valid_move(const Gameboard::Position& position) const noexcept;

            void set_white_moves(const std::vector<Gameboard::Position>& moves);
            void set_colored_moves(const std::vector<Gameboard::Position>& moves);
            void blank_hovering();
            void blank_white();
            void blank_colored();
            void set_hovering(const Gameboard::Position& position);
            void set_white(const Gameboard::Position& position);
            void set_colored(const Gameboard::Position& position);
            void clear_moves();
            void clear_hovering();
            void clear_white();
            void clear_colored();

            bool locked_white() const noexcept;
            bool locked_colored() const noexcept;

            void check_hovering(const sf::Vector2f& mouse_pos);
            void check_clicked(const sf::Vector2f& mouse_pos);
            std::pair<Gameboard::Position, Gameboard::Position> end_turn();

            /* void set_player(Player* p); */
            void set_leader(bool l);
            void take_penalty();
            void set_font(sf::Font* fnt);
            GbBox& get_box(Gameboard::Position position);

            ///\effects Given a `y_offset` fills in the positions
            /// of the individual `GbBox`'s of the board
            /// \returns the y_postition of the bottom edge of the board
            float fill_positions(float y_offset);
            void draw(sf::RenderWindow& window);
            void update();
            void print_marks() const;
            


        private:
            typedef std::array<GbBox, gameboard_row_length> GbRow;

            std::array<GbRow, game_num_colors> rows;
            std::array<unsigned, game_num_colors> marks {0, 0, 0, 0};
            std::array<GbBox, penalties_needed_to_end> penalties;
            unsigned penalty_count = 0;

            std::vector<Gameboard::Position> viable_white_moves;
            std::vector<Gameboard::Position> viable_colored_moves;

            Gameboard::Position hovering{Gameboard::Color::red, Gameboard::Number::lock};
            Gameboard::Position white{Gameboard::Color::red, Gameboard::Number::lock};
            Gameboard::Position colored{Gameboard::Color::red, Gameboard::Number::lock};
            sf::Font* font = nullptr;

            bool leader = false;
            bool taken_white = false;
            bool taken_colored = false;
            bool lock_white = false;
            bool lock_colored = false;
    };
}


