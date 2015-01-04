// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#ifndef INCLUDE_ARGCV_GRAPH_VERTEX_H_
#define INCLUDE_ARGCV_GRAPH_VERTEX_H_

#include <stdio.h>

#include <string>
#include <vector>
#include <map>
#include <set>
#include <utility>
// #include <thread>  // std::thread
#include <mutex>  // std::mutex

#include "leveldb/db.h"
#include "leveldb/cache.h"
#include "leveldb/options.h"
#include "leveldb/write_batch.h"

#include "argcv/util/oid.h"
#include "argcv/graph/edge.h"


namespace argcv {
namespace graph {

class Edge;

class Vertex {
 private :
    class Agent {
     public:
        Agent(Vertex & m_n, const std::string & m_k) : n(m_n), k(m_k) {}
        operator std::string() const { return n.get(k);}
        void operator = (const std::string & v) { n.set(k, v); }

     private:
        Vertex &n;
        const std::string &k;
    };

 public :
    Vertex() { _valid = false; }
    Vertex(const std::string & m_id,
            leveldb::DB *m_db,
            std::mutex *m_gg_mtx):
        _id(m_id), db(m_db), _gg_mtx(m_gg_mtx){}
    virtual ~Vertex() {}
    Agent operator[] (const std::string &k) {return Agent(*this, k);}

    const std::string & id() const { return _id; }
    const std::vector<Vertex> inV(const std::string & val="", const std::string & field = "name") const;
    const std::vector<Vertex> outV(const std::string & val="", const std::string & field = "name") const;
    const std::vector<Edge> inE(const std::string & val="", const std::string & field = "label") const;
    const std::vector<Edge> outE(const std::string & val="", const std::string & field = "label") const;
    const std::vector<std::string> keys() const ;
    bool drop();
    const std::set<std::string> drop_set() const ;
    bool valid();

    const std::string get(const std::string & k);  // get value by field
    void set(const std::string &k, const std::string &v, bool index = true);  // set value by field

 private :
    bool _valid = true;
    const std::string _id;
    leveldb::DB* db;
    std::mutex * _gg_mtx;  // mutex for critical section
};


}  // namespace graph
}  // namespace argcv

#endif  // INCLUDE_ARGCV_GRAPH_VERTEX_H_
