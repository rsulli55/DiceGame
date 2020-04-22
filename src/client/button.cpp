#include "button.h"

namespace qdg {
    Button::Button(sf::Font* fnt) :
        Box::Box(color_dark_grey, sf::Color::White,
                sf::Vector2f{button_width, button_height})
        {
            set_font(fnt);
        }
}

