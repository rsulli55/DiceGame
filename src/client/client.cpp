#include <imgui.h>
#include <imgui-SFML.h>
#include <SFML/Graphics.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#include "../gamestate.h"
#include "../netmessage.h"


const auto server_ip = sf::IpAddress("192.168.0.249");
const auto server_port = (unsigned short) 54'000;
int main() {

    sf::Socket::Status status = socket.connect(server_ip, server_port);
    if (status != sf::Socket::Done) {
        spdlog::error("Failed setting up socket, exiting\n");
        exit(1);
    }


    return 0;
}
