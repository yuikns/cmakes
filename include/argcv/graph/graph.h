// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#ifndef INCLUDE_ARGCV_GRAPH_GRAPH_H_
#define INCLUDE_ARGCV_GRAPH_GRAPH_H_

#include <stdint.h>  // uint64_t

#include <string>
#include <vector>
#include <set>

// #include <thread>  // std::thread
#include <mutex>  // std::mutex

#include "leveldb/db.h"
#include "leveldb/cache.h"
#include "leveldb/options.h"

#include "edge.h"
#include "vertex.h"

namespace argcv {
namespace graph {

class Graph {
 public :
    Graph(const std::string &m_ddir, uint64_t cache_sz = 0 );
    virtual ~Graph();
    bool err() const { return err_flag; }

    Edge link(const std::string &from_id,
            const std::string & to_id,
            bool index = true);

    Vertex new_v();
    Vertex id_v(const std::string &id);

    Vertex find_one_v(const std::string & val, const std::string & field="name");
    Edge find_one_e(const std::string & val, const std::string & field="label");
    
    std::vector<Vertex> find_v(const std::string & val, const std::string & field="name");
    std::vector<Edge> find_e(const std::string & val, const std::string & field="label");

    void test();
    void dump();

 private :
    Graph();  // Private constructor
    Graph(const Graph &);  // Prevent copy-construction
    const std::string ddir;  // dict dir
    uint64_t cache_size;
    leveldb::DB* db;
    leveldb::Options options;
    std::mutex * _gg_mtx;  // mutex for critical section
    bool err_flag;
    void init();
};

}  // namespace graph
}  // namespace argcv

#endif  // INCLUDE_ARGCV_GRAPH_GRAPH_H_
