//
// Created by juicylucy on 4/21/23.
//

#include "GameEntities.h"
#include <vector>
#include <algorithm>

void GameEntities::AddTexture(const char *file) {
    textures.push_back(GameTexture(file));
}

void GameEntities::AddEntity(const BaseEntity &entity1) {
    entity.push_back(entity1);

}

void GameEntities::AddEntity(float x, float y, int texture) {
    entity.push_back(BaseEntity(x,y,texture));
}

int GameEntities::numEntities() const {
    return entity.size();
}

void GameEntities::SyncSize() {
    entities.resize(entity.size());
}

void GameEntities::AddTexture(const GameTexture &gameTexture) {
    textures.push_back(gameTexture);
}

void GameEntities::SortSprites() {
    std::sort(entities.rbegin(),entities.rend());
}
