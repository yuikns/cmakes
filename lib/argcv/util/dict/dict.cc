// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#include "argcv/util/dict.h"

#include <cstdint>  // uint64_t
#include <cstdio>  // fgets

// #include <iostream>
#include <algorithm>
#include <string>
// #include <fstream>

#include "argcv/util.h"

#include "leveldb/db.h"
#include "leveldb/cache.h"
#include "leveldb/options.h"

namespace argcv {
namespace util {

std::string dict_default_key("argcv::dict::freq::");
std::string dict_default_db_path("dict.db");

Dict::Dict() {
    // std::cout << "dict start" << std::endl;
    // db init
    options.create_if_missing = true;  // 如果没有则创建
    options.block_cache = leveldb::NewLRUCache(30 * 1048576L);  // 1000MB cache

    leveldb::Status status =
        leveldb::DB::Open(options, dict_default_db_path, &db);
    assert(status.ok());
}

Dict::~Dict() {
    // std::cout << "dict destory" << std::endl;
    delete db;
    delete options.block_cache;
}


bool Dict::build(const std::string & file_path) {
    // std::string line;
    char line[1024];
    // std::ifstream doc (file_path);
    FILE * f = fopen(file_path.c_str(), "rb");
    size_t off = 0;
    // if (doc.is_open()) {
    if ( f != NULL ) {
        while (1) {
            if (fgets(line, 1024, f) == NULL )
                break;
            std::vector<std::string> kv = argcv::util::split(line, " ");

            if (kv.size() == 2) {
                off++;
                if ((db->Put(leveldb::WriteOptions(),
                                dict_default_key+kv[0],
                                kv[1])).ok()) {
                    // fprintf(stdout, "[INFO] %s => %s PUT OK \n",
                    //  (dict_default_key+kv[0]).c_str(),kv[1].c_str() );
                } else {
                    fprintf(stderr, "[ERROR] insert failed : %s\t#\t%s\n",
                                                kv[0].c_str(), kv[1].c_str());
                }
                if (off % 10000 == 0) {
                    fprintf(stdout, "[INFO] %lu ...\n", off);
                }
            } else {
                fprintf(stderr, "[ERROR] error line : %s", line);
            }
        }
        // doc.close();
        fclose(f);
    } else {
        fprintf(stderr, "[ERROR] open file (%s) failed\n", file_path.c_str() );
        return false;
    }
    return true;
}

int Dict::level(const std::string & key) {
    std::string s_val;
    if (!(db->Get(leveldb::ReadOptions(), dict_default_key+key, &s_val)).ok()) {
        // std::cout << "key : " << line << " val : " << " N/A" << std::endl;
        return -1;
    } else {
        return s_val.length();
    }
}

uint64_t Dict::freq(const std::string & key) {
    std::string s_val;
    if (!(db->Get(leveldb::ReadOptions(), dict_default_key+key, &s_val)).ok()) {
        // std::cout << "key : " << line << " val : " << " N/A" << std::endl;
        return -1;
    } else {
        // std::stoi(s_val)
        // std::stol(long), std::stof(float), std::stod(double)
        return std::stol(s_val);
    }
}


}  // namespace util
}  // namespace argcv
