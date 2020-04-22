#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "../game_constants.h"

namespace qdg {
    
    class Box {

        public:
            enum class State { Blank, Hovered, Filled};
        Box(sf::Color fill, sf::Color hover, sf::Vector2f size);

        void fill();
        void blank();
        void hover();
        bool check_hover(const sf::Vector2f mouse_pos);
        bool check_clicked(const sf::Vector2f mouse_pos);
        void set_font(sf::Font* fnt);
        void set_label(const std::string& label);
        void set_pos(sf::Vector2f pos);
        void recenter_label();
        void draw(sf::RenderWindow& window);
        sf::FloatRect bounds();

        State state;

        private:

        sf::RectangleShape box;
        sf::Font* font = nullptr;
        sf::Text text;
        sf::Color fill_color;
        sf::Color hover_color;



    };
}
