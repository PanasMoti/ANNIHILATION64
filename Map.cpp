//
// Created by juicylucy on 4/11/23.
//

#include "Map.h"

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
            os << j;
        }
        os << "\n";
    }
    return os;
}
