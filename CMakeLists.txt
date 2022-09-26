cmake_minimum_required(VERSION 3.23)
project(R-Type)

set(CMAKE_CXX_STANDARD 17)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

execute_process(COMMAND conan install ..
    WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build")


add_executable(Server "${CMAKE_CURRENT_SOURCE_DIR}/server/src/main.cpp")

add_executable(Client "${CMAKE_CURRENT_SOURCE_DIR}/client/src/main.cpp")

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup()

target_link_libraries(Server ${CONAN_LIBS})

message("=== Conan Libraries: ===")
message(${CONAN_LIBS})
message("========================")


# execute_process(COMMAND mv build/Server build/bin/Server && mv build/Client .)

# execute_process(
#     COMMAND cmake --build .
#     WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}/build"
# )
