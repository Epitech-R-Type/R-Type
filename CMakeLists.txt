cmake_minimum_required(VERSION 3.17)
project(R-Type)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_C_COMPILER "gcc")
set(CMAKE_CXX_COMPILER "g++")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_executable(Server "${CMAKE_CURRENT_SOURCE_DIR}/server/src/main.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/shared/MessageQueue.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/server/src/Communication.cpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/server/src/Server.cpp")

add_executable(Client "${CMAKE_CURRENT_SOURCE_DIR}/client/src/main.cpp")

target_link_libraries(Server ${CONAN_LIBS})

message("=== Conan Libraries: ===")
message(${CONAN_LIBS})
message("========================")