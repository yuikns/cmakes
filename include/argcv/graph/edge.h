// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#ifndef INCLUDE_ARGCV_GRAPH_EDGE_H_
#define INCLUDE_ARGCV_GRAPH_EDGE_H_


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
#include "argcv/graph/vertex.h"


namespace argcv {
namespace graph {

class Vertex;

class Edge {
 private :
    class Agent {
     public:
        Agent(Edge & m_e, const std::string & m_k) : e(m_e), k(m_k) {}
        operator std::string() const { return e.get(k);}
        void operator = (const std::string & v) { e.set(k, v); }

     private:
        Edge &e;
        const std::string &k;
    };
 public :
    Edge() { _valid = false; }
    Edge(const std::string &m_from_id,
            const std::string &m_to_id,
            leveldb::DB *m_db,
            std::mutex *m_gg_mtx) :
                from_id(m_from_id),
                to_id(m_to_id),
                db(m_db),
                _gg_mtx(m_gg_mtx) {
        ft_id.assign(from_id + to_id);
        // tf_id.assign(to_id + from_id);
    }
    virtual ~Edge() {}
    const std::string &from() const { return from_id; }
    const std::string &to() const { return to_id; }
    const std::string &ft() const {return ft_id; }
    Agent operator[] (const std::string &k) { return Agent(*this, k); }
    const std::vector<std::string> keys() const ;
    bool drop();
    std::set<std::string> drop_set();
    bool valid() { return _valid; }

    const std::string get(const std::string & k);
    void set(const std::string &k, const std::string &v, bool index = true);

 private :
    bool _valid = true;
    const std::string from_id;
    const std::string to_id;
    std::string ft_id;
    // std::string tf_id;
    leveldb::DB *db;
    std::mutex *_gg_mtx;  // mutex for critical section
};



}  // namespace graph
}  // namespace argcv

#endif  // INCLUDE_ARGCV_GRAPH_EDGE_H_
