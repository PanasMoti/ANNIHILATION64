#pragma once

//! config file with simple #DEF for the project, define NDEBUG here for the project to be compiled as RELEASE


// uncomment to disable assert()
//#define NDEBUG

#include <cassert>
#define WIDTH 1920
#define HEIGHT 1080
#define TITLE "ANNIHILATION64"

//#ifdef NDEBUG
#define DB_PATH "data/storage.db"
//#else
//#define DB_PATH "../data/storage.db"
//#endif



/// static function to generate a random string made up from upper case letters and 0-9 digits
/// \note this is later used to generate passwords for custom made games
/// @param length unsigned long integer that contains the size of the generated string
/// \return a randomized string
static std::string random_string( size_t length )
{

    auto randchar = []() -> char
    {
        const char charset[] =
                "0123456789"
                "ABCDEFGHIJKLMNOPQRSTUVWXYZ";
        const size_t max_index = (sizeof(charset) - 1);
        return charset[ rand() % max_index ];
    };
    std::string str(length,0);
    std::generate_n( str.begin(), length, randchar );
    return str;
}


