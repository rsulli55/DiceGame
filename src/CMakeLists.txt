add_library(gameboard OBJECT gameboard.cpp)

find_package(spdlog)
find_package(sfml)
target_link_libraries(gameboard spdlog::spdlog sfml::sfml)

add_library(die OBJECT die.cpp)
target_link_libraries(die spdlog::spdlog sfml::sfml)

# add_library(utility OBJECT utility.cpp)
# target_link_libraries(utility spdlog::spdlog sfml::sfml)

add_library(player OBJECT player.cpp)
target_link_libraries(player sfml::sfml gameboard)

add_library(gamestate OBJECT gamestate.cpp utility.cpp)
target_link_libraries(gamestate  gameboard die player)

add_library(servergamestate OBJECT servergamestate.cpp)
target_link_libraries(servergamestate  gamestate player)

add_library(netmessage OBJECT netmessage.cpp)
target_link_libraries(netmessage gameboard die player)



add_subdirectory(client)
add_subdirectory(server)
