//
// Created by juicylucy on 4/20/23.
//



#ifndef ANNIHILATION64_FILEUTIL_H
#define ANNIHILATION64_FILEUTIL_H
#include <sys/stat.h>
#include <unistd.h>
#include <string>
#include <fstream>

static inline bool exists_test0 (const std::string& name) {
    std::ifstream f(name.c_str());
    return f.good();
}

static inline bool exists_test1 (const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

static inline bool exists_test2 (const std::string& name) {
    return ( access( name.c_str(), F_OK ) != -1 );
}

static inline bool exists_test3 (const std::string& name) {
    struct stat buffer;
    return (stat (name.c_str(), &buffer) == 0);
}


#include <memory>
#include <string>
#include <stdexcept>

template<typename ... Args>
std::string string_format( const std::string& format, Args ... args )
{
    int size_s = std::snprintf( nullptr, 0, format.c_str(), args ... ) + 1; // Extra space for '\0'
    if( size_s <= 0 ){ throw std::runtime_error( "Error during formatting." ); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf( new char[ size ] );
    std::snprintf( buf.get(), size, format.c_str(), args ... );
    return std::string( buf.get(), buf.get() + size - 1 ); // We don't want the '\0' inside
}


#endif //ANNIHILATION64_FILEUTIL_H
