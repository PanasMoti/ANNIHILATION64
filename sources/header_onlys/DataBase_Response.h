//
// Created by elias on 17/04/2023.
//

#ifndef ANNIHILATION64_DATABASE_RESPONSE_H
#define ANNIHILATION64_DATABASE_RESPONSE_H

#include <string>


///@class Singleton class that is used to hold the response from a database as std::string
///@see "Singleton is a design pattern that ensures that a class can only have one object"
///@remark this class is a Singleton because this game is meant to be a @b STRICTLY @b SINGLE @b PLAYER @b GAME and the database is stored @b LOCALLY
///which means that at any point there won't be more than 1 query to the database
///@note if we were to extend the game to have multiplayer or access the database through the web we will need to change this class
///@attention due to the way the sqlite3.h works, to save the query's response a global/static/extern variable is needed, as
/// the function callback has predetermined parameters and only returns an integer that represents error code of the query
class DataBase_Response {

public:

    /// @brief a function to retrieve a reference to the only object of this class
    /// \return a refernce of the only instance of this class
    // @note : i called this function `self` since its shorter than `instance`
    static DataBase_Response& self() {
        static DataBase_Response baseResponse;
        return baseResponse;
    }

    //! @property std::string used to store the response of a database query
    std::string str;

    // functions that used to make this class a Singleton class since C++ doesn't have a specific type of classes of singleton
    DataBase_Response(const DataBase_Response&) = delete;
    DataBase_Response(DataBase_Response&&) = delete;
    DataBase_Response& operator=(DataBase_Response&&) = delete;
    DataBase_Response& operator=(const DataBase_Response&) = delete;

private:
    //! @property a static pointer to the instance of this class
    static DataBase_Response* instance;
    //! @attention the constructor is private to make this class a Singleton
    DataBase_Response() = default;
};

/// a function that will be used to retrieve the response of the query
/// @see sqlite3.h documentations
/// \param data this one is we don't use as it is optional and ignored in our case
/// \param argc the number of columns in the result set
/// \param argv the row's data
/// \param azColName the column names
/// \return 0 (this is used for error handling)
static int callback(void* data,int argc,char **argv,char **azColName) {
    int i;
    //! @var a refernce to the only instance of the Singleton class
    DataBase_Response& tmp = DataBase_Response::self();
    //! @brief for each column in the result set we combine the column name with the data of the row
    for(i = 0; i<argc; i++){
        tmp.str += std::string(azColName[i]) + " = " +  (argv[i] ? argv[i] : "NULL") + std::string("\n");
    }
    return 0;
}


#endif //ANNIHILATION64_DATABASE_RESPONSE_H
