#include <catch2/catch.hpp>
#include "../src/gameboard.h"

TEST_CASE("Testing gameboard score calculation", "[gameboard]") {
    qdg::Gameboard gameboard;
    gameboard.reset();

    REQUIRE(gameboard.calculate_score() == sf::Uint32(0));

    SECTION("Single color, one check mark") {
    gameboard.mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::two});
    REQUIRE(gameboard.calculate_score() == sf::Uint32(1));
    }

    SECTION("Single color, multiple checkmarks") {
    gameboard.mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::two});
    gameboard.mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::three});
    REQUIRE(gameboard.calculate_score() == sf::Uint32(3));
    gameboard.mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::four});
    REQUIRE(gameboard.calculate_score() == sf::Uint32(6));
    gameboard.mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::five});
    gameboard.mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::six});
    REQUIRE(gameboard.calculate_score() == sf::Uint32(15));
    gameboard.mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::seven});
    gameboard.mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::eight});
    gameboard.mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::nine});
    gameboard.mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::ten});
    gameboard.mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::eleven});
    REQUIRE(gameboard.calculate_score() == sf::Uint32(55));
    gameboard.mark_position({qdg::Gameboard::Color::yellow, qdg::Gameboard::Number::twelve});
    REQUIRE(gameboard.calculate_score() == sf::Uint32(78));
    }

    SECTION("Multiple colors, one check mark") {
    gameboard.mark_position({qdg::Gameboard::Color::yellow, 
            qdg::Gameboard::Number::two});
    gameboard.mark_position({qdg::Gameboard::Color::red, 
            qdg::Gameboard::Number::two});
    gameboard.mark_position({qdg::Gameboard::Color::blue, 
            qdg::Gameboard::Number::twelve});
    gameboard.mark_position({qdg::Gameboard::Color::green, 
            qdg::Gameboard::Number::twelve});
    REQUIRE(gameboard.calculate_score() == sf::Uint32(4));
    }
}

    
TEST_CASE("Testing gameboard valid_move", "[gameboard]") {
    qdg::Gameboard gameboard;
    gameboard.reset();
    int total_colors = 4;
    int total_number = 11; //we do not use Number::lock explicitly

    SECTION("No check marks, all positions should be valid except locking number") {
        for (int c = 0; c < total_colors; ++c) {
            for (int n = 0; n < total_number; ++n) {
                qdg::Gameboard::Color color = static_cast<qdg::Gameboard::Color>(c);
                qdg::Gameboard::Number number = static_cast<qdg::Gameboard::Number>(n);
                if ( (number == qdg::Gameboard::Number::twelve &&
                    (color == qdg::Gameboard::Color::red || 
                     color == qdg::Gameboard::Color::yellow)) ||
                    (number == qdg::Gameboard::Number::two &&
                    (color == qdg::Gameboard::Color::green || 
                     color == qdg::Gameboard::Color::blue)) ) {
                    
                    REQUIRE(!gameboard.valid_move({color, number}));
                }

                else {
                    REQUIRE(gameboard.valid_move({color, number}));
                }
            }

        }
    }

    SECTION("If red is locked, no red move should be valid") {
        gameboard.mark_position({ qdg::Gameboard::Color::red, 
                qdg::Gameboard::Number::twelve });

        auto color = qdg::Gameboard::Color::red;

        for (int n = 0; n < total_number; ++n) {
            qdg::Gameboard::Number number = static_cast<qdg::Gameboard::Number>(n);
            if (gameboard.valid_move({color, number})) {
                spdlog::error("Number {} is incorrect\n", n);
            }
            REQUIRE(!gameboard.valid_move({color, number}));
        }
        
    }
}



