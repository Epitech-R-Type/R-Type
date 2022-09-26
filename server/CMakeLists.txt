cmake_minimum_required(VERSION 3.23)
project(B_CPP_500_BER_5_1_rtype_nicolas_lattemann)

set(CMAKE_CXX_STANDARD 17)

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()

add_executable(R-Type src/main.cpp)

target_link_libraries(R-Type ${CONAN_LIBS})