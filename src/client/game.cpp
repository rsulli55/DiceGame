#include "game.h"


namespace qdg {

    Game::Game() : window(sf::VideoMode(window_width, window_height), 
            "DiceGame", sf::Style::Titlebar | sf::Style::Close)
    {
        window.setFramerateLimit(60);
        view = window.getDefaultView();
        window.setView(view);
        view_min_y = view.getCenter().y;

        //set font
        font.loadFromFile(std::string(path_to_assets) + "kenvector_future_thin.ttf");
        player_board.set_font(&font);
        transition_message.setFont(font);
        host_message.setFont(font);
        host_message.scale(2.f, 2.f);
        host_message.setPosition({side_spacing, top_spacing});

        //load new turn sound
        new_turn_sb.loadFromFile(std::string(path_to_assets) + "newturn.wav");
        new_turn_sound.setBuffer(new_turn_sb);

        //position stuff
        current_y_offset = position_player_turn();
        current_y_offset = std::max(current_y_offset, position_dice());
        current_y_offset = position_player_board(current_y_offset + height_spacing);
        current_y_offset = position_button(current_y_offset + height_spacing);

        transition_message.setString("Setting Up ...");
        auto rect = transition_message.getGlobalBounds();
        transition_message.setOrigin(rect.width / 2.f, rect.height);
        transition_message.setPosition(window_width / 2.f, window_height / 2.f);
        transition_message.setScale(2.f, 2.f);
        window.clear();
        window.draw(transition_message);
        window.display();
        load_config();
    }

    void Game::load_config() {
        std::ifstream config_file {std::string(path_to_assets) + "config.txt"};
        if (!config_file) {
            spdlog::error("Could not load config, make sure it is in the correct location\n");
            spdlog::error("Continuing with default server_ip and server_port\n");
            return;
        }

        std::string buffer, ip;
        unsigned short port;

        config_file >> buffer >> ip;
        spdlog::debug("buffer: {}, ip: {}\n", buffer, ip);
        gameclient.set_server_ip(ip);
        config_file >> buffer >> port;
        spdlog::debug("buffer: {}, port: {}\n", buffer, port);
        gameclient.set_server_port(port);
    }


    void Game::connect_to_server() {
        fmt::print("Connecting to server...\n");
        sf::Time time_to_wait = sf::seconds(2.f);
        sf::Time elapsed;
        button.set_label("Try again");
        host_message.setString("Connecting to server...");
        bool connected = false;
        bool keep_trying = true;
        bool show_button = false;

        auto mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        while (!connected && keep_trying && window.isOpen()) {
            if (!show_button) {
                connected = gameclient.connect_to_server();
            }

            elapsed += clock.restart();
            if (elapsed >= time_to_wait) {
                host_message.setString("Could not connect.\nTry again?");
                show_button = true;
            }

            sf::Event event;
            mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            if (show_button) 
                button.check_hover(mouse_pos);
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    keep_playing = false;
                }

                else if (event.type == sf::Event::MouseButtonPressed) {
                    if (show_button && button.check_clicked(mouse_pos)) {
                        keep_trying = true;
                        show_button = false;
                        elapsed = sf::Time::Zero;
                        button.blank();
                        host_message.setString("Connecting to server ...");
                    }
                }
            }

            window.clear();
            window.draw(host_message);

            if (show_button) button.draw(window);
            window.display();
        }

        host_message.setString("Connected!");
        window.clear();
        window.draw(host_message);
        window.display();
        
        auto message = gameclient.receive_message();

        switch(message.msg_type) {
            case NetMessage::Type::set_host: 
                spdlog::debug("Received set host message\n");
                player_id = message.player_id;
                host = true;
                break;
            case NetMessage::Type::not_host: 
                spdlog::debug("Received not host message\n");
                player_id = message.player_id;
                host = false;
                break;
            default:
                spdlog::error("Received unexpected message connecting to server\n");
                break;
        }

        gameclient.set_id(player_id);
        gameclient.set_host(host);
        fmt::print("Connected!\n");
        gameclient.set_non_blocking();
    }

    void Game::prepare_screen() {
        std::string player_name;
        sf::String prompt{"Enter your name: "};
        sf::Text input_field;
        if (host) {
            host_message.setString("You are the host!\nMake sure all are ready!");
            host_message.setFillColor(sf::Color::Red);
        }
        else {
            host_message.setString("Waiting for the host...");
        }
        input_field.setFont(font);
        input_field.setScale(1.5f, 1.5f);
        input_field.setPosition(side_spacing, window_height / 2.5f);
        input_field.setString(prompt);
        bool submitted = false;
        bool host_ready = false;
        button.set_label("Submit");

        //mouse stuff
        auto mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        bool in_button = false;

        while(window.isOpen() && !game_running) {
            sf::Event event;
            mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            button.check_hover(mouse_pos);
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    keep_playing = false;
                }

                else if (event.type == sf::Event::TextEntered) {
                    if (event.text.unicode != 0x000008) {
                        player_name += event.text.unicode;
                    }
                    else {
                        player_name = player_name.substr(0, player_name.size()-1);
                    }
                    input_field.setString(prompt + player_name);
                }

                else if (event.type == sf::Event::MouseButtonPressed) {
                    if (button.check_clicked(mouse_pos)) {
                        if (!submitted) {
                            submitted = true;
                            gameclient.add_player(player_name);
                            if (host) {
                                button.set_label("Ready");
                                button.blank();
                            }
                                
                        }
                        else if (host && !host_ready) {
                            host_ready = true;
                            gameclient.set_ready();
                        }
                    }
                }
            }

            auto message = gameclient.receive_message();
            handle_prep_message(message);
            gameclient.send_message();

            window.clear();
            if (!submitted) {
                window.draw(input_field);
            }
            else
                window.draw(host_message);
            button.draw(window);
            window.display();
        }

        fmt::print("Game is running!");
    }

    void Game::handle_prep_message(NetMessage& message) {
        switch(message.msg_type) {
            case NetMessage::Type::no_msg:
                break;
            /*Pregame Messages*/
            case NetMessage::Type::add_player: {
                spdlog::debug("Adding player with id {} and name {}\n",
                        message.player_id, message.player_name);
                auto& id = message.player_id;
                auto& name = message.player_name;
                add_player(id, name);
                break;
            }
            case NetMessage::Type::start_game:
                spdlog::debug("Received start game message\n");
                /// Managers network communications from the server sidedd
                leader_id = message.player_id;
                game_running = true;
                break;
            default:
                spdlog::error("Got unexpected message in prepare_screen\n");
                break;
        }
    }

    float Game::position_player_turn() {
        players_turn.setPosition({side_spacing, top_spacing});
        auto rect = players_turn.getGlobalBounds();
        //text height is actually 2 times rect height?
        return rect.height * 2.f + top_spacing;
    }

    float Game::position_button(float y_offset) {
        y_offset += height_spacing;
        button.set_pos({window_width - side_spacing - button.button_width_with_border,
                       y_offset});
        return y_offset + button.button_height_with_border;
    }

    float Game::position_dice() {
        float x_offset = window_width - side_spacing - 5.f * width_spacing;
        x_offset -= 6.f * dice_manager.die_side;
        for (auto& color : all_die_colors) {
            dice_manager.set_die_pos(color, {x_offset, top_spacing});
            x_offset += dice_manager.die_side + width_spacing;
        }

        //die height
        return dice_manager.die_side + top_spacing;
    }

    float Game::position_player_board(float y_offset) {
        return player_board.fill_positions(y_offset);

    }

    float Game::position_opp_boards(float y_offset) {
        for (auto i = 0; i < opp_boards.size(); ++i) {
            opp_names[i].setPosition(side_spacing, y_offset);
            //height of the text is only half?
            y_offset += opp_names[i].getGlobalBounds().height * 2.f + height_spacing;
            y_offset = opp_boards[i].fill_positions(y_offset);
            y_offset += height_spacing;
        }

        return y_offset;
    }

    void Game::run_game() {
        //only position opp boards on the first game
        if (!replaying)
            current_y_offset = position_opp_boards(current_y_offset + height_spacing);
        view_max_y = current_y_offset - window_height / 2.f;
        new_round();
        sf::Time delta;

        while(game_running && window.isOpen()) {
            auto mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            button.check_hover(mouse_pos);
            player_board.check_hovering(mouse_pos);
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    keep_playing = false;
                }

                else {
                    handle_event(event, mouse_pos);
                }
            }

            auto message = gameclient.receive_message();
            if (message.msg_type != NetMessage::Type::no_msg) {
                handle_message(message);
            }
            gameclient.send_message();

            delta = clock.restart();

            if (!moves_known && all_die_received && dice_manager.done_rolling()) {
                moves_known = true;
                player_board.set_white_moves(dice_manager.calc_white_moves());
                if (leader)
                    player_board.set_colored_moves(dice_manager.calc_colored_moves());
            }
                
            dice_manager.update_dice(delta);

            update_view();
            window.clear();
            draw_everything();

            if (show_transition) {
                window.draw(transition_message);
                transition_elapsed += delta;
                if (transition_elapsed >= transition_interval)
                    show_transition = false;
            }
            window.setView(view);
            window.display();
        }

    }


    void Game::handle_message(NetMessage& message) {
        switch (message.msg_type) {
            /*Game Message*/
            case NetMessage::Type::roll_dice: 
                spdlog::debug("Received roll dice message\n");
                dice_manager.roll_dice();
                break;
            case NetMessage::Type::set_die:  {
                spdlog::debug("Received set die message {}\n", message);
                auto& die = message.die;
                dice_manager.set_end_face(die);
                die_received(die.color);
                break;
            }
            case NetMessage::Type::mark_position:
                spdlog::debug("Received mark position message {}\n", message);
                //lock the color if necessary
                if (opp_boards[message.player_id].mark_position(message.position)) {
                    colors_to_lock.push_back(message.position.color);
                }
                //after player moves grey out their name
                opp_names[message.player_id].setFillColor(color_dark_grey);
                break;
            case NetMessage::Type::take_penalty: 
                spdlog::debug("Received take penalty message {}\n", message);
                opp_boards[message.player_id].take_penalty();
                //after player moves grey out their name
                opp_names[message.player_id].setFillColor(color_dark_grey);
                break;
            case NetMessage::Type::new_round:
                spdlog::debug("Received new round message {}\n", message);
                leader_id = message.player_id;
                new_round();
                break;
            case NetMessage::Type::game_over: 
                spdlog::debug("Received game over message\n");
                game_running = false;
                //clear out colors to be locked
                colors_to_lock.clear();
                break;
            case NetMessage::Type::disconnect: 
/* TODO: Figure out about removing players */
                spdlog::debug("Received disconnect message\n");
                state.remove_player(message.player_id);
                break;
            default: 
                spdlog::debug("Unexpected msg_type in handle_message()\n");
                break;
        }
    }

    void Game::handle_event(sf::Event &event, const sf::Vector2f& mouse_pos) {

        if (event.type == sf::Event::KeyPressed) {
            switch(event.key.code) {
                case sf::Keyboard::Down:
                    scrolling_down = true;
                    break;
                case sf::Keyboard::Up:
                    scrolling_up = true;
                    break;
            }
        }

        else if (event.type == sf::Event::KeyReleased) {
            switch(event.key.code) {
                case sf::Keyboard::Down:
                    scrolling_down = false;
                    break;
                case sf::Keyboard::Up:
                    scrolling_up = false;
                    break;
            }
        }

        else if (event.type == sf::Event::MouseWheelScrolled) {
            view.move({0.f, -30.f * event.mouseWheelScroll.delta});

            if (view.getCenter().y > view_max_y) 
                view.setCenter(view.getCenter().x, view_max_y);
            else if (view.getCenter().y < view_min_y) 
                view.setCenter(view.getCenter().x, view_min_y);
        }

        else if (event.type == sf::Event::MouseButtonPressed) {
            if (!ended_turn) {
                player_board.check_clicked(mouse_pos);
            }

            if (button.state != Box::State::Filled && button.check_clicked(mouse_pos)) {
                if (leader && !rolled_dice) {
                    rolled_dice = true;
                    update_button();
                    gameclient.roll_dice();
                }
                else {
                    end_turn();
                    ended_turn = true;
                }
            }
        }

    }

    void Game::update_player_turn() {
        auto leader_turn = opp_names[leader_id].getString();
        leader_turn += "'s\nTurn";
        players_turn.setString(leader_turn);
    }

    void Game::update_button() {
        button.blank();
        if (leader && !rolled_dice) {
            button.set_label("Roll Dice");
        }

        else 
            button.set_label("End Turn");
    }

    void Game::new_round() {
        ended_turn = false;
        all_die_received = false;
        moves_known = false;
        std::fill(std::begin(received_die), std::end(received_die), false);
        rolled_dice = false;
        check_leader();
        update_player_turn();
        update_button();
        button.blank();
        player_board.set_leader(leader);
        player_board.clear_moves();
        new_round_transition();
        new_turn_sound.play();

        for (auto& color : colors_to_lock)  {
            spdlog::debug("Locking color {}\n", color);
            dice_manager.lock_color(color);
        }
        colors_to_lock.clear();

        for (auto& [id, name] : opp_names) {
            name.setFillColor(sf::Color::White);
        }
    }

    void Game::end_turn() {
        spdlog::debug("In end_turn()\n");
        auto [p1, p2] = player_board.end_turn();
        spdlog::debug("Pos1 {}, Pos2 {}\n", p1, p2);

        bool marked_something = false;

        if (p1.number != Gameboard::Number::lock) {
            gameclient.mark_position(p1);
            marked_something = true;
        }

        if (leader && p2.number != Gameboard::Number::lock) {
            gameclient.mark_position(p2);
            marked_something = true;
        }

        if (leader && !marked_something) {
            gameclient.take_penalty();
        }

        gameclient.end_turn();
    }


    void Game::draw_everything() {

        window.draw(players_turn);
        dice_manager.draw_dice(window);
        player_board.draw(window);
        button.draw(window);

        for (auto& [id, oppboard] : opp_boards) {
            window.draw(opp_names[id]);
            oppboard.draw(window);
        }
    }


    void Game::update_view() {
      if (scrolling_down) {
          view.move(0.f, 10.f);
          if (view.getCenter().y >= view_max_y) {
              view.setCenter(view.getCenter().x, view_max_y);
              scrolling_down = false;
          }
      }

      else if (scrolling_up) {
          view.move(0.f, -10.f);
          if (view.getCenter().y <= view_min_y) {
              view.setCenter(view.getCenter().x, view_min_y);
              scrolling_up = false;
          }
            
      }
    }

    void Game::check_leader() {
        if (player_id == leader_id) {
            spdlog::debug("We are the leader\n");
            leader = true;
        }
        else {
            spdlog::debug("We are NOT the leader\n");
            leader = false;
        }

    }
    
    void Game::die_received(Die::Color color) {
        auto c = static_cast<unsigned>(color);

        received_die[c] = true;

        if (std::all_of(std::cbegin(received_die), std::cend(received_die),
                    [](bool b) { return b; })) 
        {
            spdlog::debug("All dice have been received\n");
            all_die_received = true;

        }
    }

    void Game::add_player(sf::Uint8 id, std::string name) {

        //add opponent name
        opp_names.insert({id, sf::Text{name, font}});

        //add opponent board
        GbBoard opp_board;
        opp_board.set_font(&font);
        /* opp_board.set_player(&state.get_player(id)); */
        opp_boards.insert({id, opp_board});
    }
    
    void Game::new_round_transition() {
        show_transition = true;
        transition_message.setString(opp_names[leader_id].getString() + "'s\nTurn");
        transition_message.setScale(2.f, 2.f);
        /* transition_message.setString("New round!"); */
        transition_message.setFillColor(sf::Color::Red);
        auto rect = transition_message.getGlobalBounds();
        transition_message.setOrigin(rect.width / 2.f, rect.height);
        transition_message.setPosition(window_width / 2.f, window_height / 2.f);
        transition_elapsed = sf::Time::Zero;
        spdlog::debug("New round and leader is id {}\n", leader_id);
    }

    void Game::end_game_screen() {
        player_board.print_marks(); 
        button.set_label("Replay");
        button.blank();

        std::string results_string;
        sf::Text results_text;

        results_string += fmt::format("{:13}\t\t{:13}\n", "Player", "Score");
        for (auto& [id, opp_name] : opp_names) {
            auto name = opp_name.getString().toAnsiString();
            fmt::print("{:13}\t\t{:13}\n", name, opp_boards[id].calculate_score());
            results_string += fmt::format("{:13}\t\t{:13}\n", name, opp_boards[id].calculate_score());
        }
        results_text.setString(results_string);
        results_text.setFont(font);
        results_text.setPosition(side_spacing, 15.f* top_spacing);

        if (host) {
            host_message.setString("You are the host!\nMake sure all are ready!");
            host_message.setFillColor(sf::Color::Red);
        }

        replaying = false;
        
        auto mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
        while(window.isOpen() && !game_running) {
            mouse_pos = window.mapPixelToCoords(sf::Mouse::getPosition(window));
            button.check_hover(mouse_pos);
            sf::Event event;
            while (window.pollEvent(event)) {
                if (event.type == sf::Event::Closed) {
                    window.close();
                    keep_playing = false;
                }
                
                else if (event.type == sf::Event::MouseButtonPressed) {
                    if (!replaying && button.check_clicked(mouse_pos)) {
                        replaying = true;
                        //reset boards
                        player_board.reset();
                        for (auto& [id, opp_board] : opp_boards) {
                            opp_board.reset();
                        }

                        //reset dice
                        dice_manager.reset();

                        if (!host) {
                            host_message.setString("Waiting for host ...");
                            host_message.setFillColor(sf::Color::White);
                        }

                        else {
                            gameclient.set_ready();
                        }
                    }
                }
            }
            
            auto message = gameclient.receive_message();
            handle_prep_message(message);
            gameclient.send_message();
            
            window.clear();
            if (host || replaying)
                window.draw(host_message);
            window.draw(results_text);
            button.draw(window);
            window.display();
        }

    }

    void Game::replay_game() {
    }

}
