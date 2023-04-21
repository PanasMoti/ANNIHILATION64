//
// Created by juicylucy on 4/16/23.
//

#ifndef ANNIHILATION64_GAMEDATA_H
#define ANNIHILATION64_GAMEDATA_H
#include "../import/linalg.h"
#include "../Map/Map.h"

using namespace linalg::aliases;

#define SHOOTINGCOOLDOWN 0.5f

struct Player {
    float2 pos;
    float2 dir;
    float2 plane;
    int hp;
    int ammo;
    float cooldown;
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
        res = {384,216};
        player.pos = static_cast<const linalg::vec<float, 2>>(map.GetPlayerSpawn());
        player.dir = {-1,0};
        player.plane = {0,0.66};
        player.ammo = 50;
        player.cooldown = SHOOTINGCOOLDOWN;
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
