cmake_minimum_required(VERSION 3.15)

# for LSP
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# add MacOS frameworks path for LLVM 17
set(CMAKE_CXX_COMPILER 
    /opt/homebrew/opt/llvm/bin/clang++ -F$ENV{SDKROOT}/System/Library/Frameworks
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "-Wall -Wno-unused-result -pedantic-errors")

project(
    a3
    LANGUAGES CXX
    VERSION 0.1.0
)

add_subdirectory(dependencies)

# make executable app
set(SRC_FILES
    src/main.cpp
    src/Action.cpp
    src/Animation.cpp
    src/Assets.cpp
    src/Physics.cpp
    src/Scene.cpp
    src/Scene_Menu.cpp
    src/Scene_Play.cpp
    src/Vec2.cpp
    src/GameEngine.cpp
    src/Entity.cpp
    src/EntityManager.cpp
)
set(exec_name a3)
add_executable(${exec_name} ${SRC_FILES})
target_link_libraries(${exec_name} PRIVATE ImGui-SFML::ImGui-SFML)
include_directories(include)

# copy assets to binary directory
file(
    COPY ${CMAKE_SOURCE_DIR}/assets/fonts/TwilioSansMNerdFont-Regular.ttf 
    DESTINATION ${CMAKE_BINARY_DIR}/assets/fonts
)

file(
    COPY ${CMAKE_SOURCE_DIR}/assets/config.txt 
    DESTINATION ${CMAKE_BINARY_DIR}/assets
)
