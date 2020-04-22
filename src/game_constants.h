#pragma once 
#include <SFML/Graphics/Color.hpp>
#include <array>
#include <SFML/Config.hpp>

namespace qdg{
    constexpr int game_num_colors = 4;
    constexpr int gameboard_row_length = 12;
    constexpr int number_of_dice = 6;
    constexpr int die_number_of_faces = 6;
    constexpr int locked_needed_to_end = 2;
    constexpr int penalties_needed_to_end = 4;

    //can have 0 - 12 checkmarks per row
    constexpr std::array<sf::Uint32, gameboard_row_length + 1> 
        score_map {0, 1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 66, 78};


    /* GRAPHICS */
    //Window properties
    constexpr float window_width = 1024.f;
    constexpr float window_height = 768.f;

    //Sizes and Postions
    constexpr float width_spacing = 5.f;
    constexpr float height_spacing = 10.f;
    constexpr float side_spacing = 5.f;
    constexpr float top_spacing = 15.f;

    constexpr float gbbox_side_length = 70.f;
    constexpr float gbbox_side_with_border = 80.f;
    constexpr float gbboard_height = 4 * gbbox_side_length + 3 * height_spacing;

    //Colors
    const sf::Color color_grey {204, 204, 204};
    const sf::Color color_dark_grey {85, 85, 85};
    const sf::Color color_red {204, 23, 10};
    const sf::Color color_light_red {204, 60, 60};
    const sf::Color color_yellow {199, 196, 12};
    const sf::Color color_light_yellow {199, 196, 115};
    const sf::Color color_green {15, 140, 47};
    const sf::Color color_light_green {84, 140, 47};
    const sf::Color color_blue {34, 58, 191};
    const sf::Color color_light_blue {93, 109, 199};
}
