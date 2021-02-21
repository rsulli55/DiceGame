add_executable(tests tests.cpp gameboard_tests.cpp gamestate_tests.cpp message_tests.cpp)
find_package(Catch2)
target_link_libraries(tests Catch2::Catch2 die gameboard player gamestate servergamestate netmessage netserver netclient)

