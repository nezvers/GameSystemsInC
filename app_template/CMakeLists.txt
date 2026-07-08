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


set(ProjectName app_template)
project(${ProjectName})
set(CMAKE_C_STANDARD 99)
file(GLOB Sprite_SRC_FILES "main.c")
add_executable(${ProjectName} ${Sprite_SRC_FILES})
target_include_directories(${ProjectName} PRIVATE ${PROJECT_SOURCE_DIR}/../include)
target_link_libraries(${ProjectName} raylib)
target_compile_definitions(${ProjectName} PUBLIC SOURCE_PATH="${CMAKE_CURRENT_SOURCE_DIR}")



# Setting ASSETS_PATH
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/../../assets/") # Set the asset path macro to the absolute path on the dev machine