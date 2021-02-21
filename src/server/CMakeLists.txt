add_library(netserver OBJECT netserver.cpp)
target_link_libraries(netserver netmessage gamestate servergamestate)
add_executable(server server.cpp)
target_link_libraries(server gameboard player netmessage die gamestate servergamestate netserver)
