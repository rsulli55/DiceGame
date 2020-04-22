# include "gbbox.h"

namespace qdg {
    GbBox::GbBox(sf::Color fill, sf::Color hover, const std::string& label) :
        Box::Box(fill, hover, sf::Vector2f{gbbox_side_length, gbbox_side_length})
        {
            set_label(label);
        }
}
