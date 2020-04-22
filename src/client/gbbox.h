#pragma once

#include <SFML/Graphics/RectangleShape.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

#include "../game_constants.h"
#include "box.h"

namespace qdg {
    
    class GbBox : public Box {

        public:
        GbBox(sf::Color fill, sf::Color hover, const std::string& label);
        private:

    };
}
            
