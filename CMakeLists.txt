cmake_minimum_required(VERSION 3.16)
project(ANNIHILATION64)

set(CMAKE_CXX_STANDARD 17)


# Find SDL2 library
find_package(SDL2 2.26.1 REQUIRED)
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
IF(WIN32)
    set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_LIST_DIR}/appicon.rc")
ELSE()
    set(APP_ICON_RESOURCE_WINDOWS "")
ENDIF()

add_compile_options(-static-libstdc++ -static-libgcc)
set(CMAKE_EXE_LINKER_FLAGS  "-static-libstdc++ -static-libgcc")

add_executable(${PROJECT_NAME} WIN32 sources/main.cpp
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
        "sources/State_GameOver.cpp" "sources/State_GameOver.h"
        sources/gfx/GameEntities.cpp sources/gfx/GameEntities.h
        ${APP_ICON_RESOURCE_WINDOWS}
        )


# Link libraries
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf SQLite::SQLite3 nlohmann_json::nlohmann_json)
add_custom_target(copy_assets
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
        )
add_custom_target(copy_data
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data
        )
add_dependencies(${PROJECT_NAME} copy_assets copy_data)



    include(InstallRequiredSystemLibraries)
    include(GNUInstallDirs)
    set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/License.txt")
    install(FILES "$<TARGET_FILE:SDL2::SDL2>" DESTINATION bin)
    install(FILES "$<TARGET_FILE:SDL2_image::SDL2_image>" DESTINATION bin)
    install(FILES "$<TARGET_FILE:SDL2_ttf::SDL2_ttf>" DESTINATION bin)
    install(FILES "$<TARGET_FILE:SQLite::SQLite3>" DESTINATION bin)
    file(GLOB missing_dll "dll/*.dll" )
    install(FILES ${missing_dll} DESTINATION bin)
    install(FILES ${CMAKE_INSTALL_SYSTEM_RUNTIME_LIBS} DESTINATION bin COMPONENT Libraries)


    install(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/assets" DESTINATION bin)
    install(DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/data" DESTINATION bin)

    set(CPACK_PACKAGE_VERSION_MAJOR "0")
    set(CPACK_PACKAGE_VERSION_MINOR "1")
    install(TARGETS ${PROJECT_NAME} RUNTIME DESTINATION bin)
    include(CPack)
