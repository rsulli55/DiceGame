#include "dicemanager.h"

namespace qdg {
    DiceManager::DiceManager(const std::string& path_to_assets) {
        
        load_textures(path_to_assets);
        load_sounds(path_to_assets);
        dice_roll_sound.setBuffer(dice_roll_sb);

        for (int i = 0; i < number_of_dice; ++i) {
            auto& die = dice[i];
            die.setup_sprite(Die::Face::one);
        }
    }

    void DiceManager::set_die_pos(const Die::Color& color, const sf::Vector2f& pos) {
        auto c = static_cast<unsigned>(color);
        dice[c].set_pos(pos);
    }

    void DiceManager::set_rendering(const Die::Color& color, bool render) {
        auto c = static_cast<unsigned>(color);
        render_state[c] = render;
        assert(dice[c].is_rolling() == false);
        spdlog::debug("Set color {} to state {}\n", color, render);
    }
            
    void DiceManager::set_end_face(const Die& die) {
        auto c = static_cast<unsigned>(die.color);
        dice[c].set_end_face(die.face);
    }

    AnimatedDie& DiceManager::get_die(const Die::Color& color) {
        auto c = static_cast<unsigned>(color);
        return dice[c];
    }

    void DiceManager::lock_color(const Gameboard::Color& color) {
        spdlog::debug("DM::lock_color\n");
        auto die_color = convert_gb_color(color);
        spdlog::debug("Locking die_color {}\n", die_color);
        set_rendering(die_color, false);
    }

    bool DiceManager::done_rolling() {
        for (auto& die : dice) {
            if (die.is_rolling())
                return false;
        }

        return true;
    }


    Die::Color DiceManager::convert_gb_color(const Gameboard::Color& color) {
        switch (color) {
            case Gameboard::Color::red:
                return Die::Color::red;
            case Gameboard::Color::yellow:
                return Die::Color::yellow;
            case Gameboard::Color::green:
                return Die::Color::green;
            case Gameboard::Color::blue:
                return Die::Color::blue;
        }
    }

    std::vector<Gameboard::Position> DiceManager::calc_white_moves() {
        std::vector<Gameboard::Position> result;
        auto f1 = static_cast<unsigned>(get_die(Die::Color::white1).face);
        auto f2 = static_cast<unsigned>(get_die(Die::Color::white2).face);
        spdlog::debug("calc_white_moves(): white dice\n");
        spdlog::debug("White1: {} {}\n", Die::Color::white1, get_die(Die::Color::white1).face);
        spdlog::debug("White2: {} {}\n", Die::Color::white2, get_die(Die::Color::white2).face);

        auto number = static_cast<Gameboard::Number>(f1 + f2);

        for (auto& color : all_gameboard_colors) {
            auto die_color = convert_gb_color(color);
            //only add colored moves for die which are unlocked (rendering)
            if (render_state[static_cast<unsigned>(die_color)]) {
                spdlog::debug("Added postion {}\n", Gameboard::Position{color, number});
                result.push_back({color, number});
            }

            else {
                spdlog::debug("Did not add postion {}\n", Gameboard::Position{color, number});
            }

        }

        return result;
    }

    std::vector<Gameboard::Position> DiceManager::calc_colored_moves() {
        std::vector<Gameboard::Position> result;
        auto f1 = static_cast<unsigned>(get_die(Die::Color::white1).face);
        auto f2 = static_cast<unsigned>(get_die(Die::Color::white2).face);
        spdlog::debug("calc_colored_moves(): Colored dice\n");

        for (auto& color : all_gameboard_colors) {
            auto die_color = convert_gb_color(color);
            //only add colored moves for die which are unlocked (rendering)
            auto color_f = static_cast<unsigned>(get_die(die_color).face);
            auto number1 = static_cast<Gameboard::Number>(f1 + color_f);
            auto number2 = static_cast<Gameboard::Number>(f2 + color_f);
            if (render_state[static_cast<unsigned>(die_color)]) {
                /* spdlog::debug("{} {}\n", die_color, get_die(die_color).face); */
                //only add colored moves which are not already white moves
                //nevermind that won't work
                /* if (color_f != f2) */
                    result.push_back({color, number1});
                /* if (color_f != f1) */
                    result.push_back({color, number2});
                    spdlog::debug("Added postions {} {}\n", Gameboard::Position{color, number1}, Gameboard::Position{color, number2});
            }
            
            else {
                spdlog::debug("Did not add postions {} {}\n", Gameboard::Position{color, number1}, Gameboard::Position{color, number2});
            }
        }

        return result;
    }

    void DiceManager::roll_dice() {
        dice_roll_sound.play();
        for (unsigned i = 0; i < dice.size(); ++i) {
            if (render_state[i]) {
            int r = roll_one_die();
            dice[i].roll(roll_times[r-1]);
            }
        }
    }

    void DiceManager::update_dice (const sf::Time& dt) {
        for (int i = 0; i < number_of_dice; ++i) {
            if (render_state[i]) {
                dice[i].update(dt);
            }
        }
    }


    void DiceManager::draw_dice (sf::RenderWindow& window) {
        for (int i = 0; i < number_of_dice; ++i) {
            if (render_state[i]) {
                dice[i].draw(window);
            }
        }
    }

    void DiceManager::reset() {
        spdlog::debug("Resetting dice");
        std::fill(std::begin(render_state), std::end(render_state), true);

        for (auto& state: render_state) {
            assert(state == true);
        }
    }

    /* void DiceManager::change_rolling(DieColor color) { */
    /*     //this function is called when the dice dialog button is clicked */
    /*     //if the dice is changed to not render, then we need to make rolling false */
    /*     int c = static_cast<int>(color); */

    /*     if (!render_state[c]) { */
    /*         dice[c].set_rolling(false); */
    /*     } */
    /* } */


    void DiceManager::load_textures(const std::string& path_to_assets) {
        for (int i = 0; i < number_of_dice; ++i) {
            auto dice_path = path_to_assets;
            dice_path.append("die/");
            dice_path.append(std::to_string(i+1));
            dice_path.append("side.png");
            if (!faces[i].loadFromFile(dice_path)) {
                spdlog::error("Error loading {}\n", dice_path);
            }

            faces[i].setSmooth(true);
        }
    }

    void DiceManager::load_sounds(const std::string& path_to_assets) {
        auto path = path_to_assets + "diceroll.wav";
        if(!dice_roll_sb.loadFromFile(path)) {
            spdlog::error("Error loading {}\n", path);
        }
    }
}
