//
// Created by elias on 17/04/2023.
//

#ifndef ANNIHILATION64_DATABASE_RESPONSE_H
#define ANNIHILATION64_DATABASE_RESPONSE_H

#include <string>

class DataBase_Response {
public:
    static DataBase_Response& self() {
        static DataBase_Response baseResponse;
        return baseResponse;
    }

    std::string str;

    DataBase_Response(const DataBase_Response&) = delete;
    DataBase_Response(DataBase_Response&&) = delete;
    DataBase_Response& operator=(DataBase_Response&&) = delete;
    DataBase_Response& operator=(const DataBase_Response&) = delete;

private:
    static DataBase_Response* instance;
    DataBase_Response() = default;
};

static int callback(void* data,int argc,char **argv,char **azColName) {
    int i;
    DataBase_Response& tmp = DataBase_Response::self();
    for(i = 0; i<argc; i++){
        tmp.str += std::string(azColName[i]) + " = " + (argv[i] ? argv[i] : "NULL") + "\n";
    }

    return 0;
}


#endif //ANNIHILATION64_DATABASE_RESPONSE_H
