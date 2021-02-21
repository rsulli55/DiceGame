#pragma once

#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/System/Time.hpp>
#include <SFML/System/Vector2.hpp>

#include "../client/netclient.h"
#include "gbboard.h"
#include "button.h"
#include "dicemanager.h"
#include "../game_constants.h"

namespace qdg {
    static constexpr const char* path_to_assets = "assets/";

    class Game {
        typedef std::array<sf::Vector2f, gameboard_row_length> row_positions;
        typedef std::array<row_positions, game_num_colors> board_positions;
        public:
        Game();
        void connect_to_server();
        void prepare_screen();
        void run_game();
        void post_game();
        void handle_event(sf::Event& event, const sf::Vector2f& mouse_pos);
        void end_game_screen();
        void replay_game();
        bool keep_playing = true;


        private:

        void load_config();

        float current_y_offset = 0.f;
        float position_dice();
        float position_player_turn();
        float position_button(float y_offset);
        float position_player_board(float y_offset);
        float position_opp_boards(float y_offset);
        void update_view();
        void update_player_turn();
        void update_button();
        void new_round();
        void end_turn();
        void draw_everything();
        void handle_prep_message(NetMessage& meesage);
        void handle_message(NetMessage& message);
        void new_round_transition();

        void die_received(Die::Color color);

        void check_leader();

        void add_player(sf::Uint8 id, std::string name);

        sf::RenderWindow window;
        sf::View view;
        bool scrolling_down = false;
        bool scrolling_up = false;
        float view_max_y = 0.f;
        float view_min_y = 0.f;
        sf::Clock clock;
        sf::Font font;
        sf::Text players_turn{"Player's\nTurn", font};
        sf::Text host_message;
        sf::SoundBuffer new_turn_sb;
        sf::Sound new_turn_sound;
        Button button{&font};
        GbBoard player_board;
        DiceManager dice_manager {path_to_assets};
        sf::Text transition_message;
        sf::Time transition_interval = sf::seconds(1.f);
        sf::Time transition_elapsed;
        bool show_transition;


        std::map<sf::Uint8, GbBoard> opp_boards;
        std::map<sf::Uint8, sf::Text> opp_names;

        NetClient gameclient;
        Gamestate state;

        sf::Uint8 player_id;
        sf::Uint8 leader_id;
        bool leader = false;
        bool rolled_dice = false;
        bool ended_turn = false;
        bool host = false;
        bool replaying = false;
        bool game_running = false;
        bool moves_known = false;
        bool all_die_received = false;
        std::array<bool, number_of_dice> received_die {
            false, false, false,
            false, false, false
        };
        std::vector<Gameboard::Color> colors_to_lock;
    };
}

