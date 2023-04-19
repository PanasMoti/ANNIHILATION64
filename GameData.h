//
// Created by juicylucy on 4/16/23.
//

#ifndef ANNIHILATION64_GAMEDATA_H
#define ANNIHILATION64_GAMEDATA_H
#include "linalg.h"
#include "Map.h"

using namespace linalg::aliases;

struct Player {
    float2 pos;
    float2 dir;
    float2 plane;
};

struct GameData {
    using u32 = std::uint32_t;
    static GameData& self() {
        static GameData instance;
        return instance;
    }
    Map map;
    int2 res;
    Player player;
    std::string level;
    bool is_new_game;
    void init() {
        res = {640,640};
        player.pos = static_cast<const linalg::vec<float, 2>>(map.GetPlayerSpawn());
        player.dir = {1,0};
        player.plane = {0,0.66  };
        is_new_game = false;
    }
    void SetMap(const Map& map1) {
        map = map1;
        init();
    }


private:
    GameData() = default;
    GameData(const GameData&) = delete;
    GameData(GameData&&) = delete;
    GameData& operator=(const GameData&) = delete;
    GameData& operator=(GameData&&) = delete;

};


#endif //ANNIHILATION64_GAMEDATA_H
