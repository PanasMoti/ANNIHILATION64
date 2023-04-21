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

# Find json library
find_package(nlohmann_json 3.2.0 REQUIRED)


# Add executable
add_executable(${PROJECT_NAME} sources/main.cpp
        sources/header_onlys/config.h
        "sources/RenderWindow.h" "sources/RenderWindow.cpp"
        sources/import/linalg.h sources/header_onlys/BaseState.h
        sources/EventManager.h sources/EventManager.cpp
        "sources/StateManager.h" "sources/StateManager.cpp"
        sources/Game.h sources/Game.cpp
        "sources/State_MainMenu.cpp" "sources/State_MainMenu.h"
        sources/gfx/Texture.cpp sources/gfx/Texture.h
        "sources/State_LoadGame.cpp" "sources/State_LoadGame.h"
        "sources/State_MapEdit.cpp" "sources/State_MapEdit.h"
        sources/Map.cpp sources/Map.h
        sources/State_GamePlay.cpp sources/State_GamePlay.h
        sources/header_onlys/GameData.h sources/header_onlys/DataBase_Response.h
        sources/header_onlys/Colors.h sources/import/picopng.h
        "sources/gfx/GameTexture.cpp" "sources/gfx/GameTexture.h"
        sources/gfx/Sprite.cpp sources/gfx/Sprite.h
        sources/header_onlys/fileutil.h
        "sources/State_GameOver.cpp" "sources/State_GameOver.h" sources/header_onlys/pcg32.h sources/gfx/GameEntities.cpp sources/gfx/GameEntities.h)


# Link libraries
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SQLite::SQLite3 nlohmann_json::nlohmann_json)
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
        )
add_custom_target(copy_data
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data
        )
add_dependencies(${PROJECT_NAME} copy_assets copy_data)
