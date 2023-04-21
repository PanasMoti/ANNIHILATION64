//
// Created by juicylucy on 4/21/23.
//

#ifndef ANNIHILATION64_GAMEENTITIES_H
#define ANNIHILATION64_GAMEENTITIES_H
#include "GameTexture.h"
#include <unordered_map>
#include <ostream>



struct BaseEntity {
    float x,y;
    int texture;
    BaseEntity() = default;
    BaseEntity(float x,float y,int texture) : x(x),y(y),texture(texture) {}
    ~BaseEntity() = default;
    BaseEntity(const BaseEntity& other) : x(other.x),y(other.y),texture(other.texture) {}
};



class GameEntities {
public:
    static GameEntities& self() {
        static GameEntities inst;
        return inst;
    }

    std::vector<GameTexture> textures;
    std::vector<BaseEntity> entity;
    std::vector<std::pair<int,float>> entities;

    GameEntities(const GameEntities&) = delete;
    GameEntities(GameEntities&&) = delete;
    GameEntities& operator=(const GameEntities&) = delete;
    GameEntities& operator=(GameEntities&&) = delete;

    void AddTexture(const char* file);
    void AddTexture(const GameTexture& gameTexture);
    void AddEntity(const BaseEntity& entity1);
    void AddEntity(float x,float y,int texture);
    void RemoveEntity(int index);
    int FindEntity(float x,float y,int texture);
    int numEntities() const;

    void SyncSize();
    void SortSprites();



private:
    GameEntities() = default;
    GameEntities* instance;
};


#endif //ANNIHILATION64_GAMEENTITIES_H
