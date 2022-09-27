cmake_minimum_required(VERSION 3.17)
project(R-Type)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

execute_process(COMMAND conan install .. --build=raylib/3.5.0 --build glfw/3.3.6
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")


add_executable(Server "${CMAKE_CURRENT_SOURCE_DIR}/server/src/main.cpp")

add_executable(Client "${CMAKE_CURRENT_SOURCE_DIR}/client/src/main.cpp")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup()

target_link_libraries(Server ${CONAN_LIBS})

message("=== Conan Libraries: ===")
message(${CONAN_LIBS})
message("========================")