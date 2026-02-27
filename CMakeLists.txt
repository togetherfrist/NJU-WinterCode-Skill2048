cmake_minimum_required(VERSION 3.10)

project(Skill2048 CXX)

set(SFML_PATH "D:/all/wintercode/SFML-3.0.2")

include_directories(
    "${SFML_PATH}/include"
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)

file(GLOB SOURCES "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCES})

target_link_directories(${PROJECT_NAME} PRIVATE "${SFML_PATH}/lib")

target_link_libraries(${PROJECT_NAME} PRIVATE
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

target_compile_definitions(${PROJECT_NAME} PRIVATE SFML_STATIC)