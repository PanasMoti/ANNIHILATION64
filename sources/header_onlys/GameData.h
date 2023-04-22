//
// Created by juicylucy on 4/16/23.
//

#ifndef ANNIHILATION64_GAMEDATA_H
#define ANNIHILATION64_GAMEDATA_H
#include "../import/linalg.h"
#include "../Map.h"

using namespace linalg::aliases;

#define SHOOTINGCOOLDOWN 0.5f
//!@brief simple struct to represent, and hold our player's data
struct Player {
    //! @property pos -> the position of the player
    float2 pos;
    //! @property dir -> the direction the player is looking at
    float2 dir;
    //! @property plane -> the plane of the camera
    float2 plane;
    //! @property hp -> the amount of health points the player has [ this ranges between 0-3 ]
    int hp;
    //! @property ammo -> the amount of bullets the player has [ this ranges between 0-50 ]
    int ammo;
    //!@property cooldown -> the amount of times between each shot our player shoots
    float cooldown;
};


//! @brief a Singleton class that holds the game data, which will be used by different states, and will there will be only one instance of this
struct GameData {
    //! @typedef declaring unsigned integer of 32 bit as u32 instead of std::uint32_t
    using u32 = std::uint32_t;
    //! @brief Get a reference of the only instance of this class
    static GameData& self() {
        static GameData instance;
        return instance;
    }
    //!@property a map object that holds the data of the level layout,player spawn position, enemy position ,etc...
    Map map;
    //!@property an int2 object that holds the buffer resolution
    //!@attention since @b raycasting , the method we use in our game to transform a 2d array into a 3d game
    //! works for every pixel on the screen, it works wonderfully on low quality,old,slow systems, such as an arcade machine from the 90's
    //! (which is when this was invented). but for our modern computers with higher resolutions such as (1920x1080)
    //! this method is very slow and unoptimized, as we are writting to a buffer on the cpu instead of directly writing to the gpu
    //! so we limit our buffer to be only 384x216 pixels and we scale it up when rendering to the screen
    int2 res;
    Player player;
    std::string level;
    bool is_new_game;
    int score;
    void init() {
        res = {384,216};
        player.pos = static_cast<const linalg::vec<float, 2>>(map.GetPlayerSpawn());
        player.dir = {-1,0};
        player.plane = {0,0.66};
        player.ammo = 50;
        player.cooldown = SHOOTINGCOOLDOWN;
        is_new_game = false;
        score = 0;
    }
    void SetMap(const Map& map1) {
        map = map1;
        init();
    }
    GameData(const GameData&) = delete;
    GameData(GameData&&) = delete;
    GameData& operator=(const GameData&) = delete;
    GameData& operator=(GameData&&) = delete;
private:
    //! @private
    GameData() = default;


};


#endif //ANNIHILATION64_GAMEDATA_H
