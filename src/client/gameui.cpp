#include "gameui.h"

namespace qdg{
    GameUI::GameUI() {
        load_configuration();
        window.create(sf::VideoMode(resolution_width, resolution_height),
                window_title);
        window.setFramerateLimit(60);
    }

        


    while (window.isOpen()) {
    sf::Event event{};
    while (window.pollEvent(event)) {
      ImGui::SFML::ProcessEvent(event);
      if (event.type == sf::Event::Closed) { window.close(); }
      if (mouse_inside && event.type == sf::Event::MouseButtonPressed) {

          //increase current_player after a roll
          ++current_player;
          current_player %= num_players;
      }
    }
