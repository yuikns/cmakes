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

namespace argcv {
namespace graph {

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

    const std::string & id() { return _id; }
    const std::vector<Vertex> inV(const std::string & val="", const std::string & field = "name");
    const std::vector<Vertex> outV(const std::string & val="", const std::string & field = "name");
    const std::vector<Edge> inE(const std::string & val="", const std::string & field = "label");
    const std::vector<Edge> outE(const std::string & val="", const std::string & field = "label");
    const std::vector<std::string> keys() const ;
    bool drop();
    std::set<std::string> drop_set();
    bool valid() { return _valid; }

    const std::string get(const std::string & k);  // get value by field
    void set(const std::string &k, const std::string &v, bool index = true);  // set value by field

 private :
    bool _valid = true;
    const std::string _id;
    leveldb::DB* db;
    std::mutex * _gg_mtx;  // mutex for critical section
};

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
