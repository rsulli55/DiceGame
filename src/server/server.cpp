#include "netserver.h"

int main() {

    spdlog::set_level(spdlog::level::debug);
    qdg::NetServer server;

    server.prepare_game();
    server.run_game();
    while (server.any_connected())
        server.play_again(); 

    

    return 0;
}
