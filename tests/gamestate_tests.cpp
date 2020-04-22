#include <catch2/catch.hpp>
#include "../src/servergamestate.h"

TEST_CASE("Game can end by penalties", "[gamestate]") {
    qdg::ServerGamestate gamestate;
    
    gamestate.add_player(0, "p0");
    gamestate.add_player(1, "p1");
    gamestate.add_player(2, "p2");

    auto& p0 = gamestate.get_player(0);
    auto& p1 = gamestate.get_player(1);
    auto& p2 = gamestate.get_player(2);

    SECTION("Game is finished once a player reaches the correct amount of penalties") {
        int num_penalties = 4;

        for (int i = 0; i < num_penalties; ++i)  {
            REQUIRE(!gamestate.check_game_over());
            p0.penalties++;
        }
        REQUIRE(gamestate.check_game_over());
    }
}

TEST_CASE("Game ends when Red and another color are locked", "[gamestate]") {
    qdg::ServerGamestate gamestate;
        
    gamestate.lock_color(qdg::Gameboard::Color::red);
    REQUIRE(!gamestate.check_game_over());


    SECTION("Game is finished once Red and Yellow locked") {
        gamestate.lock_color(qdg::Gameboard::Color::yellow);
        REQUIRE(gamestate.check_game_over());
    }

    SECTION("Game is finished once Red and Green locked") {
        gamestate.lock_color(qdg::Gameboard::Color::green);
        REQUIRE(gamestate.check_game_over());
    }
    
    SECTION("Game is finished once Red and Blue locked") {
        gamestate.lock_color(qdg::Gameboard::Color::blue);
        REQUIRE(gamestate.check_game_over());
    }
}

TEST_CASE("Game ends when Yellow and another color are locked", "[gamestate]") {
    qdg::ServerGamestate gamestate;
        
    gamestate.lock_color(qdg::Gameboard::Color::yellow);
    REQUIRE(!gamestate.check_game_over());


    SECTION("Game is finished once Yellow and Red locked") {
        gamestate.lock_color(qdg::Gameboard::Color::red);
        REQUIRE(gamestate.check_game_over());
    }

    SECTION("Game is finished once Yellow and Green locked") {
        gamestate.lock_color(qdg::Gameboard::Color::green);
        REQUIRE(gamestate.check_game_over());
    }
    
    SECTION("Game is finished once Yellow and Blue locked") {
        gamestate.lock_color(qdg::Gameboard::Color::blue);
        REQUIRE(gamestate.check_game_over());
    }
}
    

TEST_CASE("Game ends when Green and another color are locked", "[gamestate]") {
    qdg::ServerGamestate gamestate;
        
    gamestate.lock_color(qdg::Gameboard::Color::green);
    REQUIRE(!gamestate.check_game_over());


    SECTION("Game is finished once Green and Red locked") {
        gamestate.lock_color(qdg::Gameboard::Color::red);
        REQUIRE(gamestate.check_game_over());
    }

    SECTION("Game is finished once Green and Yellow locked") {
        gamestate.lock_color(qdg::Gameboard::Color::yellow);
        REQUIRE(gamestate.check_game_over());
    }
    
    SECTION("Game is finished once Green and Blue locked") {
        gamestate.lock_color(qdg::Gameboard::Color::blue);
        REQUIRE(gamestate.check_game_over());
    }
}
    
TEST_CASE("Game ends when Blue and another color are locked", "[gamestate]") {
    qdg::ServerGamestate gamestate;
        
    gamestate.lock_color(qdg::Gameboard::Color::blue);
    REQUIRE(!gamestate.check_game_over());


    SECTION("Game is finished once Blue and Red locked") {
        gamestate.lock_color(qdg::Gameboard::Color::red);
        REQUIRE(gamestate.check_game_over());
    }

    SECTION("Game is finished once Blue and Yellow locked") {
        gamestate.lock_color(qdg::Gameboard::Color::yellow);
        REQUIRE(gamestate.check_game_over());
    }
    
    SECTION("Game is finished once Blue and Green locked") {
        gamestate.lock_color(qdg::Gameboard::Color::green);
        REQUIRE(gamestate.check_game_over());
    }
}
    
