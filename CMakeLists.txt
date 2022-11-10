cmake_minimum_required(VERSION 3.17)
project(R-Type)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

if (MSVC)
    set (CMAKE_GENERATOR "Visual Studio 17" CACHE INTERNAL "" FORCE)
    add_compile_definitions(WIN32_LEAN_AND_MEAN)
    add_compile_definitions(_WIN32_WINNT=0x0601)
else()
    set (CMAKE_GENERATOR "Unix Makefiles" CACHE INTERNAL "" FORCE)
    add_compile_options(-g3 -pthread -Wall)
endif()

include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

conan_basic_setup()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/bin)

file(DOWNLOAD "https://raw.githubusercontent.com/vector-of-bool/cmrc/master/CMakeRC.cmake"
"${CMAKE_BINARY_DIR}/CMakeRC.cmake")
include("${CMAKE_BINARY_DIR}/CMakeRC.cmake")

message("=== Conan Libraries: ===")
message(${CONAN_LIBS})
message("========================")

function(build_server)
    add_executable(Server
        "${CMAKE_CURRENT_SOURCE_DIR}/src/server/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/server/Game.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/server/Systems/Factory.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/server/Systems/ArmamentSystem.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/server/Protocols/TcpServer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/server/Protocols/LobbyProtocol.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/server/Protocols/Connections.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/server/Protocols/GameProtocol.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/server/Systems/VelocitySystem.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/server/Server.cpp"

        "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/CompPool.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/Components.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/ECSManager.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/Serialization.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/MessageQueue/MessageQueue.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/Networking/UdpCommunication.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/Systems/HitboxSystem.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/Systems/JanitorSystem.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/Utilities/UUID.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/Utilities/Timer.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/Utilities/Utilities.cpp"
    )   

    target_compile_definitions(Server PUBLIC SERVER=1)
    target_link_libraries(Server ${CONAN_LIBS})
endfunction()

function(build_client)
    add_executable(Client
            "${CMAKE_CURRENT_SOURCE_DIR}/src/client/main.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/client/Client.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/client/ClientGame/ClientGame.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/client/GUI/Components/TextBox.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/client/GUI/Menus/Connection.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/client/Protocols/TcpClient.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/client/Protocols/ClientLobbyProtocol.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/client/Protocols/TcpClient.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/client/Systems/SpriteSystem.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/client/Systems/HealthSystem.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/client/Systems/MusicSystem.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/client/Systems/PlayerMovementSystem.cpp"

            "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/CompPool.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/ECSManager.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/Components.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/Networking/UdpCommunication.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/MessageQueue/MessageQueue.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/Utilities/UUID.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/Utilities/Utilities.cpp"
            "${CMAKE_CURRENT_SOURCE_DIR}/src/client/Protocols/ClientGameProtocol.cpp"
    )
    target_compile_definitions(Client PUBLIC CLIENT=1)


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
        resources/lost.png
        resources/background.png
        resources/song0.mp3
        resources/song1.mp3
        resources/gunShot1.wav
        resources/gunShot2.wav
        resources/gunShot3.wav
    )

    target_link_libraries(Client client::rc)
    target_link_libraries(Client ${CONAN_LIBS})
endfunction()


function(build_tests)
    include(FetchContent)
    FetchContent_Declare(googletest URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip)
    set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

    FetchContent_MakeAvailable(googletest)
    enable_testing()

    set(SOURCE_CODE
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/CompPool.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/ECSManager.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/Components.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/src/shared/ECS/Serialization.cpp"
    )

    set(TEST_CODE
        "${CMAKE_CURRENT_SOURCE_DIR}/test/main.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/ECS/EcsTests.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/ECS/EcsFixture.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/test/ECS/Serialization.cpp"
    )

    add_executable(
        Tests
        "${SOURCE_CODE}"
        "${TEST_CODE}"
    )

    target_link_libraries(Tests GTest::gtest_main)
    target_link_libraries(Tests ${CONAN_LIBS})
    target_link_libraries(Tests gcov)


    include(GoogleTest)

    gtest_discover_tests(Tests)
endfunction()

if(TESTMODE EQUAL 1)
    message("=== Unit Tests ===")
    set(CMAKE_CXX_FLAGS "--coverage")
    set(CMAKE_EXE_LINKER_FLAGS "--coverage")
    add_compile_definitions(GTEST)

    build_tests()
else()
    message("==== Programs ====")
    build_client()
    build_server()
endif()
