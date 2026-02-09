cmake_minimum_required(VERSION 3.10)

project(My2048 CXX)

set(SFML_PATH "D:/all/wintercode/SFML-3.0.2")

include_directories(
    "${SFML_PATH}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

add_executable(my2048
    src/main.cpp
    src/GUI.cpp
    src/Game.cpp
)

target_link_directories(my2048 PRIVATE "${SFML_PATH}/lib")

target_link_libraries(my2048 PRIVATE
    sfml-graphics-s
    sfml-window-s
    sfml-system-s
    sfml-audio-s
    opengl32
    winmm
    gdi32
    freetype
    stdc++fs
    flac
    vorbisenc
    vorbisfile
    vorbis
    ogg
)

target_compile_definitions(my2048 PRIVATE SFML_STATIC)