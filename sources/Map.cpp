//
// Created by juicylucy on 4/11/23.
//

#include "Map.h"
#include <iostream>
Map::Map(int2 _size) {
    this->size = _size;
    ///@note: as you can see we give the size in reverse order due to the nature of 2d arrays/vectors
    grid = InitalizeGrid(_size.y,_size.x);
}

Map::Map(int width, int height) {
    this->size = {width,height};
    ///@note: as you can see we give the size in reverse order due to the nature of 2d arrays/vectors
    grid = InitalizeGrid(height,width);
}

int2 Map::GetSize() const {
    return this->size;
}

int Map::GetWidth() const {
    return this->size.x;
}

int Map::GetHeight() const {
    return this->size.y;
}

void Map::SetSize(int2 _size) {
    this->size = _size;
    this->UpdateSize();

}

void Map::SetSize(int width, int height) {
    this->size = {width,height};
    this->UpdateSize();
}

void Map::SetWidth(int width) {
    this->size.x = width;
    this->UpdateSize();
}

void Map::SetHeight(int height) {
    this->size.y = height;
    this->UpdateSize();
}

CellType &Map::operator()(size_t x, size_t y) {
    ///due to the nature of a 2dimensional array/vector, the coords are used as (y,x)
    return this->grid[y][x];
}


const CellType &Map::operator()(size_t x, size_t y) const {
    ///due to the nature of a 2dimensional array/vector, the coords are used as (y,x)
    return this->grid[y][x];
}


void Map::UpdateSize() {
    ChangeSize(&this->grid,this->size.y,this->size.x);
}

std::ostream &operator<<(std::ostream &os, const Map &map) {
    for(auto& i : map.grid) {
        for(auto& j : i) {
            os << j << " ";
        }
        os << "\n";
    }
    return os;
}

bool Map::IsValidMap() const {
    // מערך מונים שסופר כמה תאים יש מכל סוג
    int counter[7] = {0}; // initialize the counting array to zero to avoid memory leak errors
    // since C/C++ doesn't initalize the values of the array, sometimes we might get an error where the some elements of the array are set to some garbage value
    for(auto row : grid) {
        for(auto cell : row) {
            int index = static_cast<int>(cell);
            counter[index]++;
        }
    }
    // the rules :
    // 1.) only 1 player spawn
    // 2.) only 1 level end
    int count_level_end =counter[static_cast<int>(CellType::LevelEnd)];
    int count_player_spawn = counter[static_cast<int>(CellType::PlayerSpawn)];
    return (count_player_spawn == 1 && count_level_end == 1);

}

int2 Map::GetPlayerSpawn() const {
    // search the grid for the player spawn cell
    for(int x = 0; x < GetWidth(); x++) {
        for(int y = 0; y < GetHeight(); y++) {
            if((*this)(x,y) == CellType::PlayerSpawn) {
                return {x,y};
            }
        }
    }
    return {1,1}; // if we dont find the player, default to 1,1
}

void Map::SetMap(const std::string &str) {
    // processing a string that represents our grid
    for(int i = 0; i < GetWidth(); i++) {
        for(int j = 0; j < GetHeight(); j++) {
            auto cell = static_cast<CellType>(str[i+j*GetWidth()]-48);
            grid[i][j] = cell;
        }
    }
}

std::vector<int2> Map::GetEnemiesSpawn() const {
    std::vector<int2> vec;
    // going through every cell in the grid and if it's an EnemySpawn type,add the coordinates to the vec
    for (int x = 0; x < GetWidth(); x++) {
        for (int y = 0; y < GetHeight(); y++) {
            if ((*this)(x, y) == CellType::EnemySpawn) {
                vec.push_back({x, y});
            }
        }
    }
    return vec;
}

std::vector<int2> Map::GetItemsSpawn() const {
    std::vector<int2> vec;
    // going through every cell in the grid and if it's an ItemSpawn type,add the coordinates to the vec
    for (int x = 0; x < GetWidth(); x++) {
        for (int y = 0; y < GetHeight(); y++) {
            if ((*this)(x, y) == CellType::ItemSpawn) {
                vec.push_back({x, y});
            }
        }
    }
    return vec;
}

int2 Map::GetLevelEnd() const {
    // search the grid for the level end cell
    for(int x = 0; x < GetWidth(); x++) {
        for(int y = 0; y < GetHeight(); y++) {
            if((*this)(x,y) == CellType::LevelEnd) {
                return {x,y};
            }
        }
    }
    return {1,1};// if we dont find the level end, default to 1,1
}

void Map::FinishLevel() {
    // storing the level_end
    int2 level_end = GetLevelEnd();
    SetSize(size); // recalling the initalize function which clears the whole grid
    (*this)(level_end.x,level_end.y) = CellType::LevelEnd; // restoring the level_end cell
}
