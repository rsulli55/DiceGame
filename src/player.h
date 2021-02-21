#pragma once

#include <string>
#include "gameboard.h"

namespace qdg {


    struct Player {
        std::string name;
        Gameboard gameboard;
        unsigned penalties = 0;


        Player();
        Player(std::string p_name);    
    };

}
