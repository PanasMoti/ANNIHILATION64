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


/// @brief an enum class that represents the what every cell in our map means
/// @note this works since we only have less than 10 cell types, if we were to have more cell types , other part of the program might crash
/// as it only expects every number to be 1 character long
enum class CellType{
    Empty = 0, // the player can move freely through this
    Wall = 1, // a wall, which blocks the player
    Door = 2, // a door , a wall that can be removed and has a different texture
    PlayerSpawn = 3, // where the player starts the level
    EnemySpawn = 4, // where enemies spawn in the level
    ItemSpawn = 5, // where items spawn in the level
    LevelEnd = 6 // the finish, where the player needs to be in order to win
};


/// this is a simple operator overloading of the cout function so we can print to the console what every cell is and get a string and not a number
/// this is useful for debugging, currently it only returns the first letter of the cell type string so we can print the whole map in the console
/// \param os a reference to an std::ostream which is what passes to the std::cout to print to the console
/// \param cellType a constant reference to a cellType object
/// \return the reference to the std::ostream
static std::ostream& operator<<(std::ostream& os, const CellType& cellType) {
    char const *rets[] = {"Empty","Wall","Door","PlayerSpawn","enemySpawn","ItemSpawn","LevelEnd"};
    return os << rets[static_cast<int>(cellType)][0];
}
/// a simple to String function that converts the cell type to an std::string for debugging and future uses
/// \param cellType a constant reference to a CellType object
/// \return an std::string with the name of the type
static std::string ToString(const CellType& cellType) {
    char const *rets[] = {"Empty","Wall","Door","PlayerSpawn","EnemySpawn","ItemSpawn","LevelEnd"};
    return rets[static_cast<int>(cellType)];
}

///@typedef defining CellGrid as 2 dimensional vector(dynamic array) of CellTypes, which is what will represent our map
using CellGrid = std::vector<std::vector<CellType>>;

/// a static function that matches every cell type to its corresponding color, this is used in the map creating screen
/// \param cell a constant reference to a CellType object
/// \return an SDL_Color object with the matching colors
static SDL_Color ToColor(const CellType& cell) {
    SDL_Color rets[] = {
            clGRAY, // Empty -> Gray
            clWHITE, // Wall -> White
            clPURPLE, // Door -> Purple
            clGREEN, // PlayerSpawn -> Green
            clRED, // EnemySpawn -> Red,
            clCYAN, // ItemSpawn -> Blue,
            clORANGE // LevelEnd -> Orange
    };
    return rets[static_cast<int>(cell)];
}


/// a static function that creates a cell grid with borders,
/// in C++ a std::vector is a dynamic array (list in Python, ArrayList in Java/C#,etc..) and starts
/// with size of 0 and we need to set the side to fit our needs, the reason we use a vector is because there are multiple options for
/// the map size and if we used a normal array we would need to deal with pointers,heap,memory leaks etc
/// \param w the width of the grid
/// \param h the height of the grid
/// \return a cell grid with width w and height h, and borders set to 1
static CellGrid InitalizeGrid(int w,int h) {
    auto vec = std::vector<std::vector<CellType>>(w,std::vector<CellType>(h,CellType::Empty));
    //setting the borders to 1 so our map is always bordered, this is because in our rendering code we assume we will eventually reach a wall
    //and the if there are no borders we would get either and overflow bugs or an infinite loop
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

/// this functions changes the grid size by creating a new grid and then simply copying it to the
/// the values the pointer points to
/// \param grid a pointer to a CellGrid that we want to change its size
/// \param w the width we want
/// \param h the height we want
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

///@class the level layout is made out of a 2d array of integers
class Map {
public:
    ///@publicsection
    //! a default constructor that simply initalize all the members to 0
    Map() = default;
    /// constructor
    /// \param _size an int2 that represents the width and height of the map
    explicit Map(int2 _size);
    /// constructor
    /// \param width the width of the map
    /// \param height the height of the map
    Map(int width,int height);
    /// Getter
    /// \return the size of the map as int2{width,height}
    int2 GetSize() const;
    /// Getter
    /// \return the width of the map
    int GetWidth() const;
    /// Getter
    /// \return the height of the map
    int GetHeight() const;
    /// Setter
    /// \param _size the size we want to set our map to as int2{width,height}
    void SetSize(int2 _size);
    /// Setter
    /// \param width the width we want to set to
    /// \param height the height we want to set to
    void SetSize(int width,int height);
    /// Setter
    /// \param width the width we want to set to
    void SetWidth(int width);
    /// Setter
    /// \param height the height we want to set to
    void SetHeight(int height);
    /// operator overloading so we can access every cell in the grid like this : `map(x,y)`
    /// \param x the x coordinate of the cell
    /// \param y the y coordinate of the cell
    /// \return a reference to the cell at (x,y) , it is a reference so we can also change it from outside
    CellType& operator()(size_t x,size_t y);
    /// operator overloading so we can access every cell in the grid like this : `map(x,y)`
    /// \param x the x coordinate of the cell
    /// \param y the y coordinate of the cell
    /// \return a @b constant reference to the cell, so it can't be changed
    /// @note: the compiler knows when to use this or the other one
    const CellType& operator()(size_t x,size_t y) const;
    /// a friend declarition of the operator overloading of the std::ostream& << operator which lets us print a map object to the console
    /// \param os a reference to an std::ostream
    /// \param map a constant reference to a Map object
    /// \return the reference to the std::stream after we printed to it the map
    friend std::ostream& operator<<(std::ostream& os, const Map& map);
    /// a map that checks if a map is valid
    /// a map is valid if its 1.) bordered, 2.) has ONE player spawn 3.) has ONE level end
    /// @true if the map is bordered,has ONE CellType::PlayerSpawn,has ONE CellType::LevelEnd
    /// @false otherwise
    bool IsValidMap() const;
    /// Getter
    /// \return returns the x and y coordinates of the player spawn cell in the grid as an int2
    int2 GetPlayerSpawn() const;
    /// Setter
    /// \param str and std::string that represents the CellGrid
    void SetMap(const std::string& str);
    /// Getter
    /// \return std::vector of int2 that represents where all the enemies spawn on the map
    std::vector<int2> GetEnemiesSpawn() const;
    /// Getter
    /// \return std::vector of int2 that represents where all the items spawn on the map
    std::vector<int2> GetItemsSpawn() const;
    /// Getter
    /// \return returns the x and y coordinates of the Level end cell in the grid as an int2
    int2 GetLevelEnd() const;

    /// we want the player to block the player from finishing the by not giving them a route to the levelend cell
    /// but if we decied the player has earned it we drop all the walls (except the borders) and the player can easily walk
    /// to the end
    /**@note: currently, we only call this function when there are no longer enemies,
     * in the first level the level end is surronded by walls and cant be accessed unless the player kills all the enemise
     * this can later be improved but as of right now it does the job*/
    void FinishLevel();
private:
    ///@privatesection
    /// a simple function that updates the size of the map,this function is called whenever a Setter is called
    /// obviusly this means only the Setters of the map size, but currently we do not have any other setters
    void UpdateSize();
    ///@property a linalg::int2 vector that holds the size of the map like this {width,height}
    int2 size;
    ///@property a CellGrid ( see previous definition ) that represents our literal map
    CellGrid grid;
};

#endif //ANNIHILATION64_MAP_H
