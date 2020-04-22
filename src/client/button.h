#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/Graphics/Text.hpp>

#include "../game_constants.h"
#include "box.h"

namespace qdg {
    
    class Button : public Box {

        public:
        Button(sf::Font* font);
/* TODO: Put these constants somewhere else */
        static constexpr float button_width = 200.f;
        static constexpr float button_width_with_border = 210.f;
        static constexpr float button_height = 75.f;
        static constexpr float button_height_with_border = 85.f;
    };
}
