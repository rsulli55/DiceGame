#include "game.h"
#include <spdlog/common.h>
#include <spdlog/spdlog.h>



int main() {
    spdlog::set_level(spdlog::level::debug);
    sf::Font font;
    font.loadFromFile(std::string(qdg::path_to_assets) + "kenvector_future_thin.ttf"); 
    qdg::GbBoard board;
    board.set_font(&font);
    board.fill_positions(qdg::top_spacing);

    //Situation1
    board.mark_position({qdg::Gameboard::Color::red, qdg::Gameboard::Number::two});
    board.mark_position({qdg::Gameboard::Color::red, qdg::Gameboard::Number::three});
    board.mark_position({qdg::Gameboard::Color::red, qdg::Gameboard::Number::four});
    board.mark_position({qdg::Gameboard::Color::red, qdg::Gameboard::Number::five});
    board.set_leader(true);
    board.set_white_moves({{qdg::Gameboard::Color::red, qdg::Gameboard::Number::seven}});
    board.set_colored_moves({
            {qdg::Gameboard::Color::red, qdg::Gameboard::Number::twelve},
            {qdg::Gameboard::Color::red, qdg::Gameboard::Number::six},
            {qdg::Gameboard::Color::red, qdg::Gameboard::Number::ten}
            });


    sf::RenderWindow window(sf::VideoMode(qdg::window_width, qdg::window_height), 
            "Situation Tests", sf::Style::Titlebar | sf::Style::Close);
    
    while(window.isOpen()) {
        auto mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        board.check_hovering(mouse_pos);
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed) {
                window.close();
            }

            else if (event.type == sf::Event::MouseButtonPressed) {
                board.check_clicked(mouse_pos);
            }
        }

        window.clear();
        board.draw(window);
        window.display();
    }

    return 0;
}
