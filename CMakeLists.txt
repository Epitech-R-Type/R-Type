cmake_minimum_required(VERSION 3.17)
project(R-Type)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
SET(CMAKE_CXX_FLAGS "-g3")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(Server
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/MessageQueue/MessageQueue.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/Communication.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/Server.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/Manager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/CompPool.cpp")

add_executable(Client
    "${CMAKE_CURRENT_SOURCE_DIR}/src/client/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/ECS.hpp")

file(DOWNLOAD "https://raw.githubusercontent.com/vector-of-bool/cmrc/master/CMakeRC.cmake"
"${CMAKE_BINARY_DIR}/CMakeRC.cmake")
include("${CMAKE_BINARY_DIR}/CMakeRC.cmake")

cmrc_add_resource_library(
    client-resources

    ALIAS client::rc
    NAMESPACE client

    resources/r-typesheet1.png
    resources/r-typesheet2.gif
    resources/r-typesheet3.gif
    resources/r-typesheet5.gif
    resources/r-typesheet7.gif
    resources/r-typesheet8.gif
    resources/r-typesheet9.gif
    resources/r-typesheet10.gif
    resources/r-typesheet11.gif
    resources/r-typesheet12.gif
    resources/r-typesheet13.gif
    resources/r-typesheet14.gif
    resources/r-typesheet15.gif
    resources/r-typesheet16.gif
    resources/r-typesheet17.gif
    resources/r-typesheet18.gif
    resources/r-typesheet19.gif
    resources/r-typesheet20.gif
    resources/r-typesheet21.gif
    resources/r-typesheet22.gif
    resources/r-typesheet23.gif
    resources/r-typesheet24.gif
    resources/r-typesheet25.gif
    resources/r-typesheet26.gif
    resources/r-typesheet27.gif
    resources/r-typesheet28.gif
    resources/r-typesheet29.gif
    resources/r-typesheet30.gif
    resources/r-typesheet31.gif
    resources/r-typesheet32.gif
    resources/r-typesheet33.gif
    resources/r-typesheet34.gif
    resources/r-typesheet35.gif
    resources/r-typesheet36.gif
    resources/r-typesheet37.gif
    resources/r-typesheet38.gif
    resources/r-typesheet39.gif
    resources/r-typesheet40.gif
    resources/r-typesheet41.gif
    resources/r-typesheet42.gif
    resources/r-typesheet43.gif
    resources/r-typesheet44.gif
)

target_link_libraries(Client client::rc)

target_link_libraries(Server ${CONAN_LIBS})
target_link_libraries(Client ${CONAN_LIBS})

message("=== Conan Libraries: ===")
message(${CONAN_LIBS})
message("========================")