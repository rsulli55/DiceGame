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

add_executable(gametest game_test.cpp game.cpp button.cpp)
target_link_libraries(gametest die gameboard player box gbboard dicemanager netmessage gamestate netclient)

# target_link_libraries(dicegame gameboard)
# find_package(fmt)
# if (TARGET fmt::fmt)
#     target_link_libraries(dicegame fmt::fmt)
# endif()
# find_package(sfml)
# if (TARGET sfml::sfml)
#     target_link_libraries(dicegame sfml::sfml)
# endif()
# target_link_libraries(dicegame ${CONAN_LIBS})
