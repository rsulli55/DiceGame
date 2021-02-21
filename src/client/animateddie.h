#pragma once

#include "../game_constants.h"
#include "../utility.h"
#include "../die.h"
#include <SFML/Graphics.hpp>

namespace qdg {

    class AnimatedDie : public Die {
        public:
            AnimatedDie(const Color c, const std::array<sf::Texture, die_number_of_faces>& faces_);

            void setup_sprite(Face face);
            sf::Sprite& get_sprite() {
                return sprite;
            }
            Face next_face(Face face);
            void roll(const sf::Time& time_to_roll); 

            void set_end_face(Face face);

            bool is_rolling() const {
                return rolling;
            }
            void set_rolling(bool b) {
                rolling = b;
            }

            void set_pos(sf::Vector2f pos);
            sf::FloatRect bounds();
            void update(const sf::Time& dt);
            void draw(sf::RenderWindow& window);
        
        private:
/* TODO: Move these constants somewhere else or maybe put in */ 
/*           config file */
            static constexpr float roll_frame_time = 0.1f;
            static constexpr float dice_scale = 0.2f;

            sf::Sprite sprite;
            bool rolling = false;

            Face end_face = Face::one;
            bool know_end_face = false;
            sf::Time roll_time = sf::seconds(0.f);
            sf::Time rolling_total = sf::seconds(0.f);
            sf::Time since_last_face = sf::seconds(0.f);
            sf::Time time_between_faces = sf::seconds(roll_frame_time);
            //each die has a reference to the texture array from DiceManager
            const std::array<sf::Texture, die_number_of_faces>& faces;

            void set_sprite_color(Color c);
            void set_face(Face new_face) {
                int f = static_cast<int>(new_face);
                sprite.setTexture(faces[f]);
                face = new_face;
            }
            void set_texture(const sf::Texture& texture) {
                sprite.setTexture(texture);
            }
    };
}
