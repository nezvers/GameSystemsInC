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


set(ProjectName Test_AutoTile)
project(${ProjectName})
set(CMAKE_CXX_STANDARD 17)
file(GLOB AutoTile_SRC_FILES "Test_AutoTile.c")
add_executable(${ProjectName} ${AutoTile_SRC_FILES})
target_link_libraries(${ProjectName} raylib)
target_compile_definitions(${ProjectName} PUBLIC SOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

set(ProjectName Test_TileMap)
project(${ProjectName})
set(CMAKE_CXX_STANDARD 17)
file(GLOB TileMap_SRC_FILES "Test_TileMap.c")
add_executable(${ProjectName} ${TileMap_SRC_FILES})
target_link_libraries(${ProjectName} raylib)
target_compile_definitions(${ProjectName} PUBLIC SOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

set(ProjectName Test_Spread)
project(${ProjectName})
set(CMAKE_CXX_STANDARD 17)
file(GLOB Spread_SRC_FILES "Test_TileMap_spread_parallax.c")
add_executable(${ProjectName} ${Spread_SRC_FILES})
target_link_libraries(${ProjectName} raylib)
target_compile_definitions(${ProjectName} PUBLIC SOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")

set(ProjectName Test_bitmask)
project(${ProjectName})
set(CMAKE_CXX_STANDARD 17)
file(GLOB Bitmask_SRC_FILES "Test_TileSet_bitmask_editor.c")
add_executable(${ProjectName} ${Bitmask_SRC_FILES})
target_link_libraries(${ProjectName} raylib)
target_compile_definitions(${ProjectName} PUBLIC SOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")



# Setting ASSETS_PATH
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
#target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets") # Set the asset path macro in release mode to a relative path that assumes the assets folder is in the same directory as the game executable