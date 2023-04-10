cmake_minimum_required(VERSION 3.10)
project(ANNIHILATION64)

set(CMAKE_CXX_STANDARD 17)

# Find SDL2 library
find_package(SDL2 REQUIRED)
#include_directories(${SDL2_INCLUDE_DIRS})

# Find SDL_image library
find_package(SDL2_image REQUIRED)
#include_directories(${SDL2_IMAGE_INCLUDE_DIRS})

# Find SDL_ttf library
find_package(SDL2_ttf REQUIRED)
#include_directories(${SDL2_TTF_INCLUDE_DIRS})

# Find sqlite3 library
find_package(SQLite3 REQUIRED)
#include_directories(${SQLITE3_INCLUDE_DIRS})

# Add executable
add_executable(${PROJECT_NAME} main.cpp
        config.h SDL_Text.h SDL_Text.cpp RenderWindow.h RenderWindow.cpp linalg.h BaseState.h EventManager.h EventManager.cpp StateManager.h StateManager.cpp Game.h Game.cpp State_MainMenu.cpp State_MainMenu.h Texture.cpp Texture.h State_LoadGame.cpp State_LoadGame.h)

# Link libraries
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf ${SQLITE3_LIBRARIES})

add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
        )
add_custom_target(copy_data
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data
        )
add_dependencies(${PROJECT_NAME} copy_assets copy_data)
