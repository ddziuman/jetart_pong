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
                src/ActorManager.cpp
                src/Random.cpp
                src/ResourceManager.cpp
)

set(DIST_DIR ${CMAKE_BINARY_DIR}/dist)

add_custom_target(dist
    COMMAND ${CMAKE_COMMAND} -E remove_directory ${DIST_DIR}
    COMMAND ${CMAKE_COMMAND} -E make_directory ${DIST_DIR}

    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:pong>
        ${DIST_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:SFML::Graphics>
        ${DIST_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:SFML::Window>
        ${DIST_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy
        $<TARGET_FILE:SFML::System>
        ${DIST_DIR}
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/assets
        ${DIST_DIR}/assets

    DEPENDS pong
)

target_compile_features(pong PRIVATE cxx_std_20)

target_link_libraries(pong PRIVATE SFML::Graphics)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT pong)
