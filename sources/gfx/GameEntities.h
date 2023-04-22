//
// Created by juicylucy on 4/21/23.
//

#ifndef ANNIHILATION64_GAMEENTITIES_H
#define ANNIHILATION64_GAMEENTITIES_H
#include "GameTexture.h"
#include <unordered_map>
#include <ostream>


///@struct Simple Entity has only position, and index of the texture it uses
/// this can be expanded later to have different types of entities
struct BaseEntity {
    float x,y;
    int texture;
    ///Constructor
    BaseEntity() = default;
    /// Constructor
    /// \param x x position
    /// \param y y position
    /// \param texture index of the texture its using
    BaseEntity(float x,float y,int texture) : x(x),y(y),texture(texture) {}
    /// Destructor
    ~BaseEntity() = default;

    /// Copy Constructor
    /// \param other constant reference to a BaseEntity to copy from
    BaseEntity(const BaseEntity& other) : x(other.x),y(other.y),texture(other.texture) {}
};


///@class Singleton Class that has all the entites of the game in a vector, an array of the textures of the entities
/// this way there is only 1 texture loaded for every entity type
/// this also has a vector that used to sort the entites from closest to most distant for the rendering process
class GameEntities {
public:
    /// Getter
    /// \return a reference to the only instance of the class
    static GameEntities& self() {
        static GameEntities inst;
        return inst;
    }

    std::vector<GameTexture> textures;
    std::vector<BaseEntity> entity;
    std::vector<std::pair<int,float>> entities;


    //Singleton must have class
    GameEntities(const GameEntities&) = delete;
    //Singleton must have class
    GameEntities(GameEntities&&) = delete;
    //Singleton must have class
    GameEntities& operator=(const GameEntities&) = delete;
    //Singleton must have class
    GameEntities& operator=(GameEntities&&) = delete;

    /// adds a texture to the texture vector
    /// \param file path to an image
    void AddTexture(const char* file);
    /// adds a texture to the texture vector
    /// \param gameTexture constant reference to a GameTexture
    void AddTexture(const GameTexture& gameTexture);
    /// adds an entity to the entity vector
    /// \param entity1 constant reference to a BaseEntity
    void AddEntity(const BaseEntity& entity1);
    /// adds an entity to the entity vector
    /// \param x x position of the entity
    /// \param y y position of the entity
    /// \param texture index of the texture of the entity
    void AddEntity(float x,float y,int texture);
    /// removes an entity from the vector
    /// \param index the index of the entity to remove
    void RemoveEntity(int index);
    /// finds an entity in the vector from the entity's attributes
    /// \param x x position of the entity
    /// \param y y position of the entity
    /// \param texture index of the texture of the entity
    /// \return the entity's index in the vector, -1 if it's not found in the vector
    int FindEntity(float x,float y,int texture);
    /// Getter
    /// \return entity.size()
    int numEntities() const;

    /// used to synchronize the sizes of the entity & entities vectors
    void SyncSize();
    /// sorts the Entities by closest to further
    void SortEntities();



private:
    /// private constructor - Singleton
    GameEntities() = default;
};


#endif //ANNIHILATION64_GAMEENTITIES_H
