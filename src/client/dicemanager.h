#pragma once

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "../game_constants.h"
#include "../gameboard.h"
#include "animateddie.h"

namespace qdg {

    class DiceManager  {

        public:
            DiceManager(const std::string& path_to_assets);

            void set_die_pos(const Die::Color& color, const sf::Vector2f& pos);
            void set_rendering(const Die::Color& color, bool render);
            void set_end_face(const Die& die);
            void lock_color(const Gameboard::Color& color);
            Die::Color convert_gb_color(const Gameboard::Color& color);
            AnimatedDie& get_die(const Die::Color& color);
            bool done_rolling();

            std::vector<Gameboard::Position> calc_white_moves();
            std::vector<Gameboard::Position> calc_colored_moves();

            void roll_dice();
            void update_dice(const sf::Time& dt);
            void draw_dice(sf::RenderWindow& window);

            void reset();


            static constexpr float die_side = 100.f;

        private:
            void load_textures(const std::string& path_to_assets);
            void load_sounds(const std::string& path_to_assets);
            std::array<sf::Texture, die_number_of_faces> faces;
            std::array<AnimatedDie, number_of_dice> dice {{ 
                {Die::Color::white1, faces},  {Die::Color::white2, faces},
                {Die::Color::red, faces},  {Die::Color::yellow, faces},
                {Die::Color::green, faces},  {Die::Color::blue, faces}
            }};
            std::array<bool, number_of_dice> render_state {
                true, true, true, true, true, true
            };

            const std::array<sf::Time, 6> roll_times {
                    sf::seconds(0.8f), sf::seconds(0.9f),
                    sf::seconds(0.95f), sf::seconds(1.f), 
                    sf::seconds(1.1f), sf::seconds(1.15f)
            };

            sf::SoundBuffer dice_roll_sb;
            sf::Sound dice_roll_sound;
    };

}
