cmake_minimum_required(VERSION 3.10)
project(DiceGame)

include(${CMAKE_BINARY_DIR}/conan_paths.cmake)
set(CMAKE_CXX_STANDARD 17)
# include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
# conan_basic_setup()

add_subdirectory(src)
add_subdirectory(tests)

set(CMAKE_INSTALL_PREFIX ${CMAKE_SOURCE_DIR})
include(GNUInstallDirs)
install(TARGETS tests server ui_test threesided gametest situationtest
        RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}/linux64)
install(DIRECTORY  ${CMAKE_SOURCE_DIR}/assets
            DESTINATION    ${CMAKE_INSTALL_BINDIR}/linux64)
