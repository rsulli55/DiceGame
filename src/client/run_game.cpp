#include "game.h"
#include <spdlog/common.h>
#include <spdlog/spdlog.h>

int main() {
    spdlog::set_level(spdlog::level::debug);
    qdg::Game game;

    game.connect_to_server();
    game.prepare_screen();
    while (game.keep_playing) {
        game.run_game();
        game.end_game_screen();
    }

    return 0;
}
