#include "gameboard.h"

/* TODO: Cleanup */

namespace qdg {
    
    /* Gameboard::Gameboard() noexcept { */ 
    /*     reset(); */
    /* } */

    void Gameboard::reset() {
        for (gameboard_row& row: gameboard) 
            row.fill(false);

        std::fill(std::begin(marks), std::end(marks), 0);
    }

    int Gameboard::count_row(const gameboard_row& row) const {
        return std::count(std::cbegin(row), std::cend(row), true);
    }

    sf::Uint32 Gameboard::row_score(const Color color) const noexcept {
        auto c = static_cast<int>(color);
        return score_map[marks[c]];
    }

    sf::Uint32 Gameboard::calculate_score() const {
        sf::Uint32 score = 0;
        auto accumulator = [&] (const Color& color) 
                    { score += row_score(color); };

        std::for_each(std::cbegin(all_colors), 
                std::cend(all_colors), accumulator);

        return score;
    }

    bool Gameboard::check_position(const Color& color, const Number& number) const  noexcept {
        auto c = static_cast<int>(color);
        int n = 0;

        switch (color) {
            //blue and green are ordered in reverse (besides lock) 
            //i.e.: 12, 11, 10, ..., 2, lock
            case Color::green:
            case Color::blue: 
                n = (number == Number::lock) ? static_cast<int>(number)\
                                 : 10 - static_cast<int>(number);
            
            break;
            default: 
                n = static_cast<int>(number);
                break;
            
        }

        return gameboard[c][n];
    }
    
    bool Gameboard::check_position(const Position& position) const noexcept {
        return check_position(position.color, position.number);
    }

    bool Gameboard::mark_position(const Color& color, const Number& number) noexcept {

        if (number == Number::lock) {
            spdlog::warn("Do not explictly mark Number::lock\n");
            return false;
        }

        auto c = static_cast<int>(color);
        int n = 0;
        bool mark_lock = false;

        switch (color) {
            //blue and green are ordered in reverse (besides lock) 
            //i.e.: 12, 11, 10, ..., 2, lock
            case Color::green:
            case Color::blue: {
              if (number == Number::two) {
                    mark_lock = true;
                }
                
                n = 10 - static_cast<int>(number);
                break;
            }
            default: {
              if (number == Number::twelve) {
                    mark_lock = true;
                }

                n = static_cast<int>(number);
                break;
            }
        }

        gameboard[c][n] = true;
        marks[c]++;

        if (mark_lock) {
            auto l = static_cast<int>(Number::lock);
            gameboard[c][l] = true;
            marks[c]++;
            return true;
        }

        //didn't lock the color
        return false;
    }

    bool Gameboard::mark_position(const Position& position) noexcept {
        return mark_position(position.color, position.number);
    }

    bool Gameboard::valid_move(const Color& color, const Number& number) const noexcept {
        auto c = static_cast<int>(color);
        auto n = static_cast<int>(number);


        //if we are at a locking number check the number of marks
        //in that row
        if ( (number == Number::twelve &&
            (color == Color::red || color == Color::yellow)) ||
            (number == Number::two &&
            (color == Color::green || color == Color::blue)) ) {

            auto c = static_cast<int>(color);
            return marks[c] >= 5;
        }
                        

        const auto& row = gameboard[c];

        for (int i = n; i < row.size(); ++i) {
            if (row[i]) {
                return false;
            }
        }

        return true;
    }

    bool Gameboard::valid_move(const Position& position) const noexcept {
        return valid_move(position.color, position.number);
    }


    /* For printing using fmt/spdlog */
    /* template<typename OStream> */
    std::ostream& operator<<(std::ostream& out, const Gameboard::Color& color) {
        switch(color) {
            case Gameboard::Color::red:
                out << "Red";
                break;
            case Gameboard::Color::yellow:
                out << "Yellow";
                break;
            case Gameboard::Color::green:
                out << "Green";
                break;
            case Gameboard::Color::blue:
                out << "Blue";
                break;
            default:
                spdlog::error("Got an unknown Color\n");
                break;
        }
        return out;
    }

    /* template<typename OStream> */
    std::ostream& operator<<(std::ostream& out, const Gameboard::Number& number) {
        auto n = static_cast<int>(number);
        out << (n+2);

        return out;
    }

    std::ostream& operator<<(std::ostream& out, const Gameboard::Position& position) {
        out << position.color << " " << position.number;
        return out;
    }
    /* template<typename OStream> */
    /* OStream& operator<<(OStream& out, const Gameboard::Position& position) { */
    /*     out << position.color << " " << position.number; */
    /*     return out; */
    /* } */

    sf::Packet& operator<<(sf::Packet& packet, const Gameboard::Color& color) {
        auto c = static_cast<int>(color);
        packet << c;
        
        return packet;
    }
    sf::Packet& operator<<(sf::Packet& packet, const Gameboard::Number& number) {
        auto n = static_cast<int>(number);
        packet << n;

        return packet;
    }

    sf::Packet& operator<<(sf::Packet& packet, const Gameboard::Position& position) {
        packet << position.color << position.number;

        return packet;
    }
    sf::Packet& operator>>(sf::Packet& packet, Gameboard::Color& color) {
        int c;
        packet >> c;
        color = static_cast<Gameboard::Color>(c);

        return packet;
    }
    sf::Packet& operator>>(sf::Packet& packet, Gameboard::Number& number) {
        int n;
        packet >> n;
        number = static_cast<Gameboard::Number>(n);

        return packet;
    }
    sf::Packet& operator>>(sf::Packet& packet, Gameboard::Position& position) {
        packet >> position.color >> position.number;

        return packet;
    }


    bool operator==(const Gameboard::Position& lhs, const Gameboard::Position& rhs) {
        return (lhs.color == rhs.color && lhs.number == rhs.number);
    }

    bool operator!=(const Gameboard::Position& lhs, const Gameboard::Position& rhs) {
        return !(lhs == rhs);
    }











}

