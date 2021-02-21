add_library(netclient OBJECT netclient.cpp)
target_link_libraries(netclient gameboard player gamestate netmessage)

add_library(box OBJECT box.cpp)
target_link_libraries(box sfml::sfml)

add_library(gbboard OBJECT gbboard.cpp gbbox.cpp)
target_link_libraries(gbboard gameboard player box)

add_library(dicemanager OBJECT dicemanager.cpp animateddie.cpp)
target_link_libraries(dicemanager die gameboard spdlog::spdlog sfml::sfml)

add_executable(situationtest situation_tests.cpp)
target_link_libraries(situationtest gameboard box gbboard)

add_executable(run_game run_game.cpp game.cpp button.cpp)
target_link_libraries(run_game die gameboard player box gbboard dicemanager netmessage gamestate netclient)
