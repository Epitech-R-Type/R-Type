cmake_minimum_required(VERSION 3.17)
project(R-Type)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

if (NOT MSVC)
    add_compile_options(-g3)
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(Server
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/MessageQueue/MessageQueue.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/Networking/UdpCommunication.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/Protocols/Connections.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/Server.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/server/Game.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/Manager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/CompPool.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/Components.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/systems/VelocitySystem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/systems/UUID.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/Networking/TcpCommunication.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/Networking/UdpCommunication.cpp"
)

add_executable(Client
    "${CMAKE_CURRENT_SOURCE_DIR}/src/client/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/client/systems/SpriteSystem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/systems/VelocitySystem.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/systems/UUID.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/CompPool.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/Manager.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/Components.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/client/ClientGame/ClientGame.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/client/Client.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/Networking/UdpCommunication.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/MessageQueue/MessageQueue.cpp"
)

file(DOWNLOAD "https://raw.githubusercontent.com/vector-of-bool/cmrc/master/CMakeRC.cmake"
"${CMAKE_BINARY_DIR}/CMakeRC.cmake")
include("${CMAKE_BINARY_DIR}/CMakeRC.cmake")

cmrc_add_resource_library(
    client-resources

    ALIAS client::rc
    NAMESPACE client

    resources/r-typesheet1.png
    resources/r-typesheet2.png
    resources/r-typesheet3.png
    resources/r-typesheet5.png
    resources/r-typesheet7.png
    resources/r-typesheet8.png
    resources/r-typesheet9.png
    resources/r-typesheet10.png
    resources/r-typesheet11.png
    resources/r-typesheet12.png
    resources/r-typesheet13.png
    resources/r-typesheet14.png
    resources/r-typesheet16.png
    resources/r-typesheet17.png
    resources/r-typesheet18.png
    resources/r-typesheet19.png
    resources/r-typesheet20.png
    resources/r-typesheet21.png
    resources/r-typesheet22.png
    resources/r-typesheet23.png
    resources/r-typesheet24.png
    resources/r-typesheet25.png
    resources/r-typesheet26.png
    resources/r-typesheet27.png
    resources/r-typesheet28.png
    resources/r-typesheet29.png
    resources/r-typesheet30.png
    resources/r-typesheet30a.png
    resources/r-typesheet31.png
    resources/r-typesheet32.png
    resources/r-typesheet33.png
    resources/r-typesheet34.png
    resources/r-typesheet35.png
    resources/r-typesheet36.png
    resources/r-typesheet37.png
    resources/r-typesheet38.png
    resources/r-typesheet39.png
    resources/r-typesheet40.png
    resources/r-typesheet41.png
    resources/r-typesheet42.png
    resources/r-typesheet43.png
    resources/r-typesheet44.png
)

target_link_libraries(Client client::rc)

target_link_libraries(Server ${CONAN_LIBS})
target_link_libraries(Client ${CONAN_LIBS})

message("=== Conan Libraries: ===")
message(${CONAN_LIBS})
message("========================")