cmake_minimum_required(VERSION 3.28)
include(FetchContent)



# Raylib
FetchContent_Declare(
    raylib
    DOWNLOAD_EXTRACT_TIMESTAMP OFF
    URL https://github.com/raysan5/raylib/archive/refs/tags/5.5.zip
)
#FetchContent_GetProperties(raylib)
set(FETCHCONTENT_QUIET NO)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games
FetchContent_MakeAvailable(raylib)

include_directories(${CMAKE_SOURCE_DIR}/..)


set(ProjectName Test_Sprite)
project(${ProjectName})
set(CMAKE_CXX_STANDARD 17)
file(GLOB Sprite_SRC_FILES "sprite_raylib.c")
add_executable(${ProjectName} ${Sprite_SRC_FILES})
target_link_libraries(${ProjectName} raylib)
target_compile_definitions(${ProjectName} PUBLIC SOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

set(ProjectName Test_SpriteRect)
project(${ProjectName})
set(CMAKE_CXX_STANDARD 17)
file(GLOB SpriteRect_SRC_FILES "sprite_rect_raylib.c")
add_executable(${ProjectName} ${SpriteRect_SRC_FILES})
target_link_libraries(${ProjectName} raylib)
target_compile_definitions(${ProjectName} PUBLIC SOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")



# Setting ASSETS_PATH
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
#target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets") # Set the asset path macro in release mode to a relative path that assumes the assets folder is in the same directory as the game executable