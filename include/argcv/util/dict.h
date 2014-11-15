// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#ifndef INCLUDE_ARGCV_UTIL_DICT_H_
#define INCLUDE_ARGCV_UTIL_DICT_H_

#include <cstdint>  // uint64_t

#include <string>
#include <vector>
#include <sstream>

#include "leveldb/db.h"
#include "leveldb/options.h"
#include "leveldb/cache.h"

namespace argcv {
namespace util {

extern std::string dict_default_key;
extern std::string dict_default_db_path;


// web dict

class Dict {
 public :

    static Dict & instance() {
        static Dict dict;
        return dict;
    }

    virtual ~Dict();

    // static Dict & instance(const std::string & dict_path);
    bool build(const std::string & file_path);
    int level(const std::string & key);
    uint64_t freq(const std::string & key);
    uint64_t operator[](const std::string & key) {
        return freq(key);
    }

 private :
    Dict();  // Private constructor
    Dict(const Dict &);  // Prevent copy-construction
    Dict&  operator=(const Dict &);  // Prevent assignment
    // const std::string m_dict_path;
    leveldb::DB* db;
    leveldb::Options options;
};

}  // namespace util
}  // namespace argcv

#endif  // INCLUDE_ARGCV_UTIL_DICT_H_
