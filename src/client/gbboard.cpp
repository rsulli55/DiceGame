#include "gbboard.h"

namespace qdg {
    GbBoard::GbBoard() : 
    rows {{
        {{
                GbBox{color_red, color_light_red, "2"}, GbBox{color_red, color_light_red, "3"}, 
                GbBox{color_red, color_light_red, "4"}, GbBox{color_red, color_light_red, "5"}, 
                GbBox{color_red, color_light_red, "6"}, GbBox{color_red, color_light_red, "7"}, 
                GbBox{color_red, color_light_red, "8"}, GbBox{color_red, color_light_red, "9"}, 
                GbBox{color_red, color_light_red, "10"}, GbBox{color_red, color_light_red, "11"}, 
                GbBox{color_red, color_light_red, "12"}, GbBox{color_red, color_light_red, "LCK"}
            }},
            {{
                GbBox{color_yellow, color_light_yellow, "2"}, GbBox{color_yellow, color_light_yellow, "3"}, 
                GbBox{color_yellow, color_light_yellow, "4"}, GbBox{color_yellow, color_light_yellow, "5"}, 
                GbBox{color_yellow, color_light_yellow, "6"}, GbBox{color_yellow, color_light_yellow, "7"}, 
                GbBox{color_yellow, color_light_yellow, "8"}, GbBox{color_yellow, color_light_yellow, "9"}, 
                GbBox{color_yellow, color_light_yellow, "10"}, GbBox{color_yellow, color_light_yellow, "11"}, 
                GbBox{color_yellow, color_light_yellow, "12"}, GbBox{color_yellow, color_light_yellow, "LCK"} 
            }}, 
            {{
                GbBox{color_green, color_light_green, "12"}, GbBox{color_green, color_light_green, "11"}, 
                GbBox{color_green, color_light_green, "10"}, GbBox{color_green, color_light_green, "9"}, 
                GbBox{color_green, color_light_green, "8"}, GbBox{color_green, color_light_green, "7"}, 
                GbBox{color_green, color_light_green, "6"}, GbBox{color_green, color_light_green, "5"}, 
                GbBox{color_green, color_light_green, "4"}, GbBox{color_green, color_light_green, "3"}, 
                GbBox{color_green, color_light_green, "2"}, GbBox{color_green, color_light_green, "LCK"} 
            }}, 
            {{
                GbBox{color_blue, color_light_blue, "12"}, GbBox{color_blue, color_light_blue, "11"}, 
                GbBox{color_blue, color_light_blue, "10"}, GbBox{color_blue, color_light_blue, "9"}, 
                GbBox{color_blue, color_light_blue, "8"}, GbBox{color_blue, color_light_blue, "7"}, 
                GbBox{color_blue, color_light_blue, "6"}, GbBox{color_blue, color_light_blue, "5"}, 
                GbBox{color_blue, color_light_blue, "4"}, GbBox{color_blue, color_light_blue, "3"}, 
                GbBox{color_blue, color_light_blue, "2"}, GbBox{color_blue, color_light_blue, "LCK"} 
            }}
    }},
    penalties {
        GbBox{color_dark_grey, color_grey, ""}, GbBox{color_dark_grey, color_grey, ""},
        GbBox{color_dark_grey, color_grey, ""}, GbBox{color_dark_grey, color_grey, ""}
    }

    {}

    void GbBoard::set_leader(bool l) {
        leader = l;
    }

    void GbBoard::clear_hovering() {
        hovering = {Gameboard::Color::red, Gameboard::Number::lock};
    }


    void GbBoard::clear_white() {
        white = {Gameboard::Color::red, Gameboard::Number::lock};
    }

    void GbBoard::clear_colored() {
        colored = {Gameboard::Color::red, Gameboard::Number::lock};
    }

    void GbBoard::check_hovering(const sf::Vector2f& mouse_pos) {
        for (auto& pos : viable_white_moves) {
            auto& box = get_box(pos);
            box.check_hover(mouse_pos);
            
        }

        if (leader) {
            for (auto& pos : viable_colored_moves) {
                auto& box = get_box(pos);
                box.check_hover(mouse_pos);
            }

        }
    }

    void GbBoard::check_clicked(const sf::Vector2f& mouse_pos) {

        if (!leader || !taken_colored) {
            for (auto& pos : viable_white_moves) {
                auto& box = get_box(pos);
                if (box.bounds().contains(mouse_pos)) {
                    if (pos == white) {
                        spdlog::debug("Blanking white\n");
                        blank_white();
                        clear_white();
                        taken_white = false;
                        return;
                    }

                    // if we are the leader then we need to not have taken white yet
                    else if (valid_move(pos) && (!leader || !taken_white)) {
                        //clear to prevent white being hovered over
                        blank_white();
                        set_white(pos);
                        taken_white = true;
                        return;
                    }
                }
            }
        }

        if (leader) {
            for (auto& pos : viable_colored_moves) {
                auto& box = get_box(pos);
                if (box.bounds().contains(mouse_pos)) {
                    if (pos == colored) {
                        blank_colored();
                        clear_colored();
                        taken_colored = false;
                        return;
                    }

                    else if (valid_move(pos)) {
                        //clear to prevent white being hovered over
                        blank_colored();
                        set_colored(pos);
                        taken_colored = true;
                        return;
                    }
                }
            }

        }
    }

    std::pair<Gameboard::Position, Gameboard::Position> GbBoard::end_turn() {
        std::pair<Gameboard::Position, Gameboard::Position> result;

        bool marked_colored = false;
        auto no_position = Gameboard::Position{Gameboard::Color::red, Gameboard::Number::lock};

        if (leader) {
            spdlog::debug("GbBoard::end_turn() : colored is {}\n", colored);
            if (colored != no_position) {
                //we should only set colored to something other than
                //no position if it is filled
                assert(get_box(colored).state == Box::State::Filled);
                spdlog::debug("It is filled, so marking it\n");
                result.second = colored;
                marked_colored = true;
                auto c = static_cast<unsigned>(colored.color);
                //adding mark_position to make sure locked is marked if needed
                //and it handles marks
                lock_colored = mark_position(colored);
            }
            else 
                result.second = {Gameboard::Color::red, Gameboard::Number::lock};
        }

        spdlog::debug("GbBoard::end_turn() : white is {}\n", white);
        if (white != no_position) {
            //we should only set white to something other than
            //no position if it is filled
            assert(get_box(white).state == Box::State::Filled);
            spdlog::debug("It is filled, so marking it\n");
            result.first = white;
            auto c = static_cast<unsigned>(white.color);
            //adding mark_position to make sure locked is marked if needed
            //and it handles marks
            lock_white = mark_position(white);
        }
        
        else  {
            result.first = {Gameboard::Color::red, Gameboard::Number::lock};
            if (leader && !marked_colored)
                take_penalty();
        }

        return result;
    }

    bool GbBoard::locked_white() const noexcept {
        return lock_white;
    }

    bool GbBoard::locked_colored() const noexcept {
        return lock_colored;
    }


    void GbBoard::reset() {
        for (auto& color : all_gameboard_colors) {
            for (auto& number : all_gameboard_numbers) {
                auto c = static_cast<unsigned>(color);
                auto n = static_cast<unsigned>(number);

                rows[c][n].blank();
            }
        }

        penalty_count = 0;
        for (auto& pen : penalties) 
            pen.blank();

        marks = {0, 0, 0, 0};
    }
    
    bool GbBoard::mark_position(const Gameboard::Position& position) noexcept {
        auto& color = position.color;
        auto& number = position.number;

        if (number == Gameboard::Number::lock) {
            spdlog::warn("Do not explictly mark Number::lock\n");
            return false;
        }

        auto c = static_cast<unsigned>(color);
        auto n = static_cast<unsigned>(number);
        bool mark_lock = false;

        switch (color) {
            //blue and green are ordered in reverse (besides lock) 
            //i.e.: 12, 11, 10, ..., 2, lock
            case Gameboard::Color::green:
            case Gameboard::Color::blue: {
              if (number == Gameboard::Number::two) {
                    mark_lock = true;
                }
                
                n = 10 - n;
                break;
            }
            default: {
              if (number == Gameboard::Number::twelve) {
                    mark_lock = true;
                }
                break;
            }
        }

        rows[c][n].fill();
        marks[c]++;

        if (mark_lock) {
            auto l = static_cast<int>(Gameboard::Number::lock);
            rows[c][l].fill();
            marks[c]++;
            return true;
        }

        //didn't lock the color
        return false;
    }

    bool GbBoard::valid_move(const Gameboard::Position& position) const noexcept {
        auto& color = position.color;
        auto& number = position.number;
        auto c = static_cast<unsigned>(color);
        auto n = static_cast<unsigned>(number);

        //if we are at a locking number check the number of marks
        //in that row
        if ( (number == Gameboard::Number::twelve &&
            (color == Gameboard::Color::red || color == Gameboard::Color::yellow)) ||
            (number == Gameboard::Number::two &&
            (color == Gameboard::Color::green || color == Gameboard::Color::blue)) ) {

            if (marks[c] >= 5) 
                return true;
            //when we are the leader it is possible we use the white move
            //and then locking with the colored move becomes possible
            //this situation is only possible if we haven't taken the colored move yet
            else if (leader && !taken_colored && marks[c] == 4) {
                if (color == Gameboard::Color::green || color == Gameboard::Color::blue) {
                    n = (number == Gameboard::Number::lock) ? n : 10 - n;
                }
                unsigned num_filled = 0;
                for (auto& num : all_gameboard_numbers) {
                    auto to_check = static_cast<unsigned>(num);
                    if (to_check != n && rows[c][to_check].state == Box::State::Filled) {
                        num_filled++;
                    }
                }

                if (num_filled >= 5) {
                    spdlog::debug("Pos {} is valid because we are leader and at least 5 boxes are filled in that row\n");
                    return true;
                }
            }

            return false;
        }
        
        
        if (color == Gameboard::Color::green || color == Gameboard::Color::blue) {
            n = (number == Gameboard::Number::lock) ? n : 10 - n;
        }

        for (auto& num : all_gameboard_numbers) {
            auto to_check = static_cast<unsigned>(num);
            if (color == Gameboard::Color::green || color == Gameboard::Color::blue) {
                to_check = (num == Gameboard::Number::lock) ? to_check : 10 - to_check;
            }

            if (to_check >= n && rows[c][to_check].state == Box::State::Filled) {
                spdlog::debug("Position {} is not a valid move because {} is filled\n", position, Gameboard::Position{color, num});
                return false;
            }
        }

        return true;
    }

    void GbBoard::set_white_moves(const std::vector<Gameboard::Position>& moves) {
        for (auto& pos : moves) {
            if (valid_move(pos)) {
                spdlog::debug("Adding pos {} to viable white moves\n", pos);
                viable_white_moves.push_back(pos);
            }


            else {
                spdlog::debug("Pos {} is not valid\n", pos);
            }

        }
    }

    void GbBoard::set_colored_moves(const std::vector<Gameboard::Position>& moves) {
        for (auto& pos : moves) {
            if (valid_move(pos)) {
                spdlog::debug("Adding pos {} to viable colored moves\n", pos);
                viable_colored_moves.push_back(pos);
            }
            
            //if you are the leader, you could possibly lock a color 
            // after using your white move, when you only have 4 marks
            else if (leader) {
                auto c = static_cast<unsigned>(pos.color);
                auto n = static_cast<unsigned>(pos.number);

                if (pos.color == Gameboard::Color::blue || 
                    pos.color == Gameboard::Color::green) {
                    n = 10 - n;
                }

                //the "locking" columns is when n == 10
                if (n == 10 && marks[c] >= 4) {
                    viable_colored_moves.push_back(pos);
                    spdlog::debug("Adding pos {} to viable colored moves because we could possibly play it after the white move\n", pos);
                }
            }
            
            else {
                spdlog::debug("Pos {} is not valid\n", pos);
            }
        }
    }

    void GbBoard::take_penalty() {
        spdlog::debug("Penalty count is {}\n", penalty_count);
        penalties[penalty_count].fill();
        penalty_count++;
        spdlog::debug("Increased penalty count to {}\n", penalty_count);
    }

    void GbBoard::clear_moves() {
        viable_colored_moves.clear();
        viable_white_moves.clear();
        clear_hovering();
        clear_white();
        clear_colored();
        taken_white = false;
        taken_colored = false;
        lock_white = false;
        lock_colored = false;
    }

    void GbBoard::blank_hovering() {
        if (hovering.number == Gameboard::Number::lock)
            return;
        auto c = static_cast<unsigned>(hovering.color);
        auto n = static_cast<unsigned>(hovering.number);

        switch(hovering.color) {
            case Gameboard::Color::green:
            case Gameboard::Color::blue:
                n = 10 - n;
                break;
            default:
                break;
        }

        //un-hover current hovering
        rows[c][n].blank();
    }


    void GbBoard::set_hovering(const Gameboard::Position& position) {
        /* spdlog::debug("Setting hovering to pos {}\n", position); */
        //set new hovering
        hovering = position;

        auto c = static_cast<unsigned>(hovering.color);
        auto n = static_cast<unsigned>(hovering.number);

        switch(hovering.color) {
            case Gameboard::Color::green:
            case Gameboard::Color::blue:
                n = 10 - n;
                break;
            default:
                break;
        }

        rows[c][n].hover();
    }

    void GbBoard::blank_white() {
        if (white.number == Gameboard::Number::lock)
            return;
        spdlog::debug("Blanking white pos {}\n", white);
        auto c = static_cast<unsigned>(white.color);
        auto n = static_cast<unsigned>(white.number);

        //may need to also blank lock
        bool lock = false;
        switch(white.color) {
            case Gameboard::Color::green:
            case Gameboard::Color::blue:
                n = 10 - n;
                lock = white.number == Gameboard::Number::two;
                break;
            default:
                lock = white.number == Gameboard::Number::twelve;
                break;
        }

        rows[c][n].blank();
        if (lock) {
            rows[c][n+1].blank();
        }
    }

    void GbBoard::set_white(const Gameboard::Position& position) {
        spdlog::debug("Setting white to pos {}\n", position);
        //set new white
        white = position;

        auto c = static_cast<unsigned>(white.color);
        auto n = static_cast<unsigned>(white.number);

        //may need to also fill lock
        bool lock = false;
        switch(white.color) {
            case Gameboard::Color::green:
            case Gameboard::Color::blue:
                n = 10 - n;
                lock = white.number == Gameboard::Number::two;
                break;
            default:
                lock = white.number == Gameboard::Number::twelve;
                break;
        }

        rows[c][n].fill();
        if (lock) {
            rows[c][n+1].fill();
        }
    }

    void GbBoard::blank_colored() {
        if (colored.number == Gameboard::Number::lock)
            return;
        //un-set current colored
        spdlog::debug("Blanking colored pos {}\n", colored);
        auto c = static_cast<unsigned>(colored.color);
        auto n = static_cast<unsigned>(colored.number);

        //may need to also blank lock
        bool lock = false;
        switch(colored.color) {
            case Gameboard::Color::green:
            case Gameboard::Color::blue:
                n = 10 - n;
                lock = colored.number == Gameboard::Number::two;
                break;
            default:
                lock = colored.number == Gameboard::Number::twelve;
                break;
        }

        rows[c][n].blank();
        if (lock) {
            rows[c][n+1].blank();
        }
    }

    void GbBoard::set_colored(const Gameboard::Position& position) {
        spdlog::debug("Setting colored to pos {}\n", position);
        //set new colored
        colored = position;

        auto c = static_cast<unsigned>(colored.color);
        auto n = static_cast<unsigned>(colored.number);

        //may need to also fill lock
        bool lock = false;
        switch(colored.color) {
            case Gameboard::Color::green:
            case Gameboard::Color::blue:
                n = 10 - n;
                lock = colored.number == Gameboard::Number::two;
                break;
            default:
                lock = colored.number == Gameboard::Number::twelve;
                break;
        }

        rows[c][n].fill();
        if (lock) {
            rows[c][n+1].fill();
        }
    }

    GbBox& GbBoard::get_box(Gameboard::Position position) {
        auto c = static_cast<unsigned>(position.color);
        auto n = static_cast<unsigned>(position.number);

        switch (position.color) {
            //blue and green are ordered in reverse (besides lock) 
            //i.e.: 12, 11, 10, ..., 2, lock
            case Gameboard::Color::green:
            case Gameboard::Color::blue: 
                n = (position.number == Gameboard::Number::lock) ? n : 10 - n;
            break;
            default: 
                break;
            
        }
        return rows[c][n];
    }

    void GbBoard::set_font(sf::Font* fnt) {
        font = fnt;

        for (auto& color : all_gameboard_colors) {
            for (auto& number : all_gameboard_numbers) {
                auto c = static_cast<unsigned>(color);
                auto n = static_cast<unsigned>(number);

                rows[c][n].set_font(fnt);
            }
        }
    }
    
    float GbBoard::fill_positions(float y_offset) {
        //gameboard rows
        float x_offset = 0;
        for (auto& color : all_gameboard_colors) {
            x_offset = side_spacing;
            for (auto& number : all_gameboard_numbers) {
                auto c = static_cast<unsigned>(color);
                auto n = static_cast<unsigned>(number);

                rows[c][n].set_pos(sf::Vector2f{x_offset, y_offset});
                x_offset += width_spacing + gbbox_side_with_border;
            }

            y_offset += gbbox_side_with_border + width_spacing;
        }

        //penalty row
        y_offset += height_spacing;
        x_offset = side_spacing;
        for (auto& pen : penalties) {
            pen.set_pos({x_offset, y_offset});
            x_offset += width_spacing + gbbox_side_with_border;
        }

        return y_offset + gbbox_side_with_border;
    }

    void GbBoard::draw(sf::RenderWindow &window) {
        for (auto& color : all_gameboard_colors) {
            for (auto& number : all_gameboard_numbers) {
                auto c = static_cast<unsigned>(color);
                auto n = static_cast<unsigned>(number);

                rows[c][n].draw(window);
            }
        }
        
        for (auto& pen : penalties) {
            pen.draw(window);
        }
    }

    void GbBoard::print_marks() const {
        fmt::print("Printing marks\n");
        for (auto& color : all_gameboard_colors) {
            auto c = static_cast<unsigned>(color);
            fmt::print("For {} : {} marks\n", color, marks[c]);
        }
    }

    sf::Uint32 GbBoard::calculate_score()  const {
        sf::Uint32 score = 0;

        for (auto& color : all_gameboard_colors) {
            auto c = static_cast<unsigned>(color);
            score += score_map[marks[c]];
        }

        score -= penalty_count * 5;
        return (score > 0) ? score : 0;
    }
}
