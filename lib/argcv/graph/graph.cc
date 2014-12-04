#include "argcv/graph/graph.h"

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
#include "argcv/graph/vertex.h"
#include "./ldb_def.h"


namespace argcv {
namespace graph {

// V: <- Vertex
// E: <- Edge
// IV: <- Index Of Vertex
// IE: <- Index of Edge

// Graph
// TODO Graph Section
Graph::Graph(const std::string & m_ddir, uint64_t cache_sz) :
        ddir(m_ddir), cache_size(cache_sz) {
    err_flag = false;
    init();
}

Graph::~Graph() {
    delete db;
    if (cache_size > 0)
        delete options.block_cache;
    delete _gg_mtx;
}

void Graph::init() {
    options.create_if_missing = true;  // create if not exists
    if (cache_size > 0)  // use cache ?
        options.block_cache = leveldb::NewLRUCache(cache_size);

    leveldb::Status status =
        leveldb::DB::Open(options, ddir, &db);
    if (!status.ok())
        err_flag = true;
    // globe mutex for graph
    _gg_mtx = new std::mutex;  // mutex for critical section
}

Edge Graph::link(const std::string & from_id,
        const std::string & to_id,
        bool index) {
    return Edge();
}

Vertex Graph::new_v() {
    std::string id = argcv::util::ObjId::instance().id();
    ldb_set(db, kVertexPrefix + id);//reg a id
    return Vertex(id, db, _gg_mtx);
}

Vertex Graph::id_v(const std::string &id) {
    return ldb_has(db, kVertexPrefix + id)? Vertex(id, db, _gg_mtx): Vertex();
}

Vertex Graph::find_one_v(const std::string & val, const std::string & field) {
    
}

Edge Graph::find_one_e(const std::string & val, const std::string & field) {
    
}

std::vector<Vertex> Graph::find_v(const std::string & val, const std::string & field) {
    
}

std::vector<Edge> Graph::find_e(const std::string & val, const std::string & field) {
}

void Graph::dump() {
    std::map<std::string,std::string> dumps;
    ldb_map_scan(db,"d:",&dumps);
    for (std::map<std::string,std::string>::const_iterator it= dumps.begin();
            it!=dumps.end();
            it++ ) {
        printf("%% [%s] -> [%s]\n", it->first.c_str(), it->second.c_str());
    }
}

void Graph::test() {
    _gg_mtx->lock();
    std::map<std::string,std::string> kvs;
    kvs["aaa"] = "aaaa";
    kvs["d:bbb"] = "xxxx";
    kvs["d:ccc"] = "yyyy";
    kvs["d:ddd"] = "zzzz";
    string data("not setted");
    //ldb_set(db,"aaa:","aaa~");
    //ldb_del(db,"aaa:");
    //ldb_batch_add(db,kvs);
    //ldb_get(db,"d:ddd",&data);
    std::map<std::string,std::string> my_kvs;
    ldb_map_scan(db,"d:",&my_kvs);
    //printf("val: %s\n",data.c_str());
    
    for (std::map<std::string,std::string>::const_iterator it= my_kvs.begin();
            it!=my_kvs.end();
            it++ ) {
        printf("ldb_map_scan %s -> %s\n", it->first.c_str(), it->second.c_str());
    }
    
    std::vector<std::string> v_keys ;
    ldb_vec_scan(db,"d:",&v_keys);
    for(unsigned i = 0 ; i < v_keys.size(); i++ ) {
        printf("ldb_vec_scan %s\n",v_keys[i].c_str());
    }
    
    _gg_mtx->unlock();
}

}  // namespace graph
}  // namespace argcv
