//
// Created by juicylucy on 4/11/23.
//

#ifndef ANNIHILATION64_MAP_H
#define ANNIHILATION64_MAP_H


#include <vector>
#include <ostream>
#include <SDL2/SDL.h>
#include "import/linalg.h"
#include "header_onlys/Colors.h"

using namespace linalg::aliases;

enum class CellType{
    Empty = 0,
    Wall = 1,
    Door = 2,
    PlayerSpawn = 3,
    EnemySpawn = 4,
    ItemSpawn = 5,
    LevelEnd = 6
};


static std::ostream& operator<<(std::ostream& os, const CellType& cellType) {
    char const *rets[] = {"Empty","Wall","Door","PlayerSpawn","enemySpawn","ItemSpawn","LevelEnd"};
    return os << rets[static_cast<int>(cellType)][0];
}
static std::string ToString(const CellType& cellType) {
    char const *rets[] = {"Empty","Wall","Door","PlayerSpawn","EnemySpawn","ItemSpawn","LevelEnd"};
    return rets[static_cast<int>(cellType)];
}


using CellGrid = std::vector<std::vector<CellType>>;

static SDL_Color ToColor(const CellType& cell) {
    SDL_Color rets[] = {
            clGRAY, // Empty -> Gray
            clWHITE, // Wall -> White
            clPURPLE, // Door -> Purple
            clLIME, // PlayerSpawn -> Green
            clRED, // EnemySpawn -> Red,
            clCYAN, // ItemSpawn -> Blue,
            clORANGE // LevelEnd -> Orange
    };
    return rets[static_cast<int>(cell)];
}

static CellGrid InitalizeGrid(int w,int h) {
    auto vec = std::vector<std::vector<CellType>>(w,std::vector<CellType>(h,CellType::Empty));
    for(int i = 0; i < w; i++) {
        vec[i][0] = CellType::Wall;
        vec[i][h-1] = CellType::Wall;
    }
    for(int i = 0; i < h; i++) {
        vec[0][i] = CellType::Wall;
        vec[w-1][i] = CellType::Wall;
    }
    return vec;
}
static void ChangeSize(CellGrid* grid,int w,int h) {
    auto vec = std::vector<std::vector<CellType>>(w,std::vector<CellType>(h,CellType::Empty));
    for(int i = 0; i < w; i++) {
        vec[i][0] = CellType::Wall;
        vec[i][h-1] = CellType::Wall;
    }
    for(int i = 0; i < h; i++) {
        vec[0][i] = CellType::Wall;
        vec[w-1][i] = CellType::Wall;
    }
    grid->clear();
    *grid = vec;
}

class Map {
public:
    Map() = default;
    explicit Map(int2 _size);
    Map(int width,int height);
    int2 GetSize() const;
    int GetWidth() const;
    int GetHeight() const;
    void SetSize(int2 _size);
    void SetSize(int width,int height);
    void SetWidth(int width);
    void SetHeight(int height);
    CellType& operator()(size_t x,size_t y);
    const CellType& operator()(size_t x,size_t y) const;
    friend std::ostream& operator<<(std::ostream& os, const Map& map);
    bool IsValidMap() const;
    int2 GetPlayerSpawn() const;
    void SetMap(const std::string& str);
    std::vector<int2> GetEnemiesSpawn() const;
    std::vector<int2> GetItemsSpawn() const;
    int2 GetLevelEnd() const;
    void FinishLevel();
private:
    void UpdateSize();
    int2 size;
    CellGrid grid;
};

#endif //ANNIHILATION64_MAP_H
