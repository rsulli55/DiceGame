#include "animateddie.h"

namespace qdg {
    AnimatedDie::AnimatedDie(const Color c, const std::array<sf::Texture, die_number_of_faces>& face_textures) : 
        faces(face_textures), Die::Die{c, Face::one} { }

    void AnimatedDie::setup_sprite(Face face) {
        int f = static_cast<int>(face);
        sprite.setTexture(faces[f]);
        sprite.setScale(dice_scale, dice_scale);
        set_sprite_color(color);
    }

    void AnimatedDie::set_sprite_color(Color c) {
        switch (c) {
            case Color::white1:
            case Color::white2:
                return;
            case Color::red:
                sprite.setColor(color_red);
                break;
            case Color::yellow:
                sprite.setColor(color_yellow);
                break;
            case Color::green:
                sprite.setColor(color_green);
                break;
            case Color::blue:
                sprite.setColor(color_blue);
                break;
        }
    }

    void AnimatedDie::roll(const sf::Time& time_to_roll) {
        rolling = true;
        roll_time = time_to_roll;
    }

    void AnimatedDie::set_end_face(Face face) {
        end_face = face;
        know_end_face = true;
    }

    Die::Face AnimatedDie::next_face(Face face) {
        int f = roll_n_sided_die(4);

        switch(f) {
            case 1:
                if (face == Face::one || face == Face::six)
                    return Face::four;
                else if (face == Face::two || face == Face::five)
                    return Face::three;
                if (face == Face::three || face == Face::four)
                    return Face::two;
                break;
            case 2:
                if (face == Face::one || face == Face::six)
                    return Face::three;
                else if (face == Face::two || face == Face::five)
                    return Face::four;
                if (face == Face::three || face == Face::four)
                    return Face::five;
                break;
            case 3:
                if (face == Face::one || face == Face::six)
                    return Face::two;
                else if (face == Face::two || face == Face::five)
                    return Face::one;
                if (face == Face::three || face == Face::four)
                    return Face::six;
                break;
            case 4:
                if (face == Face::one || face == Face::six)
                    return Face::five;
                else if (face == Face::two || face == Face::five)
                    return Face::six;
                if (face == Face::three || face == Face::four)
                    return Face::one;
                break;
            default:
                spdlog::error("next_face(): rolled outside the range [1, 4]\n");
                return Face::one;
                break;
        }
    }

    void AnimatedDie::set_pos(sf::Vector2f pos) {
        sprite.setPosition(pos);
    }

    sf::FloatRect AnimatedDie::bounds() {
        return sprite.getGlobalBounds();
    }

    void AnimatedDie::update(const sf::Time& dt) {
        if (!rolling) {
            return;
        }

        rolling_total += dt;
        since_last_face += dt;

        if (know_end_face && rolling_total >= roll_time) {
            
            rolling = false;
            know_end_face = false;
            set_face(end_face);
            rolling_total = sf::seconds(0.f);
        }

        else if (since_last_face >= time_between_faces) {
            
            since_last_face = sf::microseconds(
                    since_last_face.asMicroseconds() % time_between_faces.asMicroseconds());
            set_face(next_face(face));
        }
    }

    void AnimatedDie::draw(sf::RenderWindow &window) {
        window.draw(sprite);
    }

}
