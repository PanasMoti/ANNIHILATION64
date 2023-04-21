//
// Created by juicylucy on 4/11/23.
//

#include "Map.h"
#include <iostream>
Map::Map(int2 _size) {
    this->size = _size;
    grid = InitalizeGrid(_size.y,_size.x);
}

Map::Map(int width, int height) {
    this->size = {width,height};
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
    return this->grid[y][x];
}


const CellType &Map::operator()(size_t x, size_t y) const {
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
    int counter[7] = {0}; // initalize the counting array to zero to avoid memory leak errors
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
    for(int x = 0; x < GetWidth(); x++) {
        for(int y = 0; y < GetHeight(); y++) {
            if((*this)(x,y) == CellType::PlayerSpawn) {
                return {x,y};
            }
        }
    }
    return {1,1};
}

void Map::SetMap(const std::string &str) {
    for(int i = 0; i < GetWidth(); i++) {
        for(int j = 0; j < GetHeight(); j++) {
            auto cell = static_cast<CellType>(str[i+j*GetWidth()]-48);
            grid[i][j] = cell;
        }
    }
}
