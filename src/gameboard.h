#pragma once
#include <SFML/Network/Packet.hpp>
#include <algorithm>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/ostr.h>

#include "game_constants.h"

namespace qdg {
    
    /// Class that manages a single gameboard
    class Gameboard {
        public:
        /// Enum for valid gameboard colors
        enum class Color {red, yellow, green, blue};
        /// Enum for valid gameboard numbers
        enum class Number {two, three, four, five, six,\
                        seven, eight, nine, ten, eleven, twelve, lock};

        /// A Position consists of a [Color](standardese://*Color/) and a
        /// [Number](standardese://*Number/)
        struct Position {
            Color color;
            Number number;
        };

        //==modifiers==//
        /// \effects clear out any marks on gameboard
        /// \group modifiers
        void reset(); 

        /// \effects marks position on the gameboard
        /// \returns  true if the move locked the color otherwise false
        /// \group modifiers
        bool mark_position(const Position& position) noexcept;
        
        sf::Uint32 calculate_score() const; 
       
        /// \returns true if the position is marked on the gameboard
        bool check_position(const Position& position) const noexcept;

        /// \returns true if marking position is a valid play
        bool valid_move(const Position& position) const noexcept;

        friend std::ostream& operator<<(std::ostream& out, const Position& position);
        friend sf::Packet& operator<<(sf::Packet& packet, const Position& position);
        friend sf::Packet& operator>>(sf::Packet& packet, Position& position);
        friend bool operator==(const Position& lhs, const Position& rhs);
        friend bool operator!=(const Position& lhs, const Position& rhs);
        
        private:
        bool mark_position(const Color& color, const Number& number) noexcept;
        bool check_position(const Color& color, const Number& number) const noexcept;
        bool valid_move(const Color& color, const Number& number) const noexcept;
       
        friend std::ostream& operator<<(std::ostream& out, const Color& color);
        friend std::ostream& operator<<(std::ostream& out, const Number& number);

        friend sf::Packet& operator<<(sf::Packet& packet, const Color& color);
        friend sf::Packet& operator<<(sf::Packet& packet, const Number& number);
        
        friend sf::Packet& operator>>(sf::Packet& packet, Color& color);
        friend sf::Packet& operator>>(sf::Packet& packet, Number& number);
        
        typedef std::array<bool, gameboard_row_length> gameboard_row;
        typedef std::array<gameboard_row, gameboard_row_length> gameboard_table;
        
        gameboard_table gameboard;
        std::array<int, game_num_colors> marks {0, 0, 0, 0};

        std::array<Color, game_num_colors> all_colors {
            Color::red, Color::yellow,
            Color::green, Color::blue
        };

/* TODO: Should count_row be removed? */
        int count_row(const gameboard_row& row) const;
        sf::Uint32 row_score(const Color color) const noexcept;
    };

    static constexpr std::array<Gameboard::Color, game_num_colors> all_gameboard_colors {
            Gameboard::Color::red, Gameboard::Color::yellow,
            Gameboard::Color::green, Gameboard::Color::blue
    };

    static constexpr std::array<Gameboard::Number, gameboard_row_length> all_gameboard_numbers {
        Gameboard::Number::two, Gameboard::Number::three,
        Gameboard::Number::four, Gameboard::Number::five,
        Gameboard::Number::six, Gameboard::Number::seven,
        Gameboard::Number::eight, Gameboard::Number::nine,
        Gameboard::Number::ten, Gameboard::Number::eleven,
        Gameboard::Number::twelve, Gameboard::Number::lock
    };
}

            

        
        
        
