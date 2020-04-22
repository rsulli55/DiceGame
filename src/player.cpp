#include "player.h"

namespace qdg {
    Player::Player() {
        gameboard.reset();
    }
    Player::Player(std::string p_name): name{p_name} {
            gameboard.reset();
        }
}
