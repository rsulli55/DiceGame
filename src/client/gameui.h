#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

namespace qdg {

    constexpr int resolution_width = 800;
    constexpr int resolution_height = 600;

    class GameClient {
        public:
            GameClient();
            void handle_event();
            void update();
            void draw();
            void load_configuration();

        private:
            sf::RenderWindow window; 
            sf::Clock clock;
            sf::Clock deltaClock;
            sf::TcpSocket socket;
            sf::TcpListener listener;

    };
}
