cmake_minimum_required(VERSION 3.28)
project(jetarts_pong LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)
FetchContent_Declare(SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM)
FetchContent_MakeAvailable(SFML)

add_executable(pong 
                src/main.cpp
                src/Engine.cpp
                src/WindowManager.cpp
                src/Timer.cpp
                src/PlayerPaddle.cpp
                src/Actor.cpp
                src/ActorManager.cpp
)


target_compile_features(pong PRIVATE cxx_std_20)


target_link_libraries(pong PRIVATE SFML::Graphics) # SFML::Network or SFML::Audio modules here

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT pong)
