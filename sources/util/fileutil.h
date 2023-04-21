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

#endif //ANNIHILATION64_FILEUTIL_H
