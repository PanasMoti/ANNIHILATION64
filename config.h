#pragma once

// uncomment to disable assert()
//#define NDEBUG

#include <cassert>
#define WIDTH 1920
#define HEIGHT 1080
#define TITLE "ANNIHILATION64"



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


