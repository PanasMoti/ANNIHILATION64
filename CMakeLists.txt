cmake_minimum_required(VERSION 3.25)
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
add_executable(${PROJECT_NAME} main.cpp)

# Link libraries
target_link_libraries(${PROJECT_NAME} SDL2::SDL2 ${SDL2_IMAGE_LIBRARIES} ${SDL2_TTF_LIBRARIES} ${SQLITE3_LIBRARIES})

