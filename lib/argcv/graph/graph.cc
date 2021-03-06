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
#include "./g_utils.h"

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
        const std::string & to_id) {
    Edge e(from_id, to_id, db, _gg_mtx); // check exists 
    if(e.valid()) {
        return e;
    }else{
        bool valid = false;
        _gg_mtx->lock();
        if(ldb_has(db, kVertexPrefix + from_id) 
            && ldb_has(db, kVertexPrefix + to_id)) {
            valid = true;
            // inline bool ldb_batch_add(leveldb::DB* db, const std::map<std::string,std::string> & kvs) 
            std::string tmp_key;
            std::map<std::string,std::string> kvs;
            //tmp_key = kEdgePrefix;
            //tmp_key += from_id;
            //tmp_key += to_id;
            //kvs.insert(std::make_pair(tmp_key,""));
        
            tmp_key = kIndexVertexOutPrefix;
            tmp_key += from_id;
            tmp_key += to_id;
            kvs.insert(std::make_pair(tmp_key,""));
        
            tmp_key = kIndexVertexInPrefix;
            tmp_key += to_id;
            tmp_key += from_id;
            kvs.insert(std::make_pair(tmp_key,""));
        
            ldb_batch_add(db, kvs );
        }
        _gg_mtx->unlock();
        return valid ? Edge(from_id, to_id, db, _gg_mtx): Edge();
    }
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
    std::string index_key(kIndexVertexPrefix);
    index_key += field ;
    index_key += kPropGlue;
    index_key += gval_encode(val);
    index_key += kPropGlue;
    //inline bool ldb_get_one(leveldb::DB* db, const std::string &key, std::string *_f_key , std::string * _val) {
    std::string k;
    std::string v;
    if(ldb_get_one(db, index_key, &k, &v )) {
        // k : _id
        // value : nothing
        return id_v(k);
    } else {
        return Vertex();
    }
}

Edge Graph::find_one_e(const std::string & val, const std::string & field) {
    std::string index_key(kIndexEdgePrefix);
    index_key += field;
    index_key += kPropGlue;
    index_key += gval_encode(val);
    index_key += kPropGlue;
    //index_key += ft_id;
    std::string k;
    std::string v;
    if(ldb_get_one(db, index_key, &k, &v)) {
        // k : ft_id
        if(k.length() == kFtLen) {
            std::string from_id = k.substr(0,kOidLen);
            std::string to_id = k.substr(kOidLen,kOidLen);
            return Edge(from_id, to_id, db, _gg_mtx);
        }else {
            return Edge();
        }
    } else {
        return Edge();
    }
}

std::vector<Vertex> Graph::find_v(const std::string & val, const std::string & field) {
    std::vector<Vertex> vv;
    std::string index_key(kIndexVertexPrefix);
    index_key += field ;
    index_key += kPropGlue;
    index_key += gval_encode(val);
    index_key += kPropGlue;
    std::vector<std::string>  ks;
    //printf("[%s_%d]find v: %s\n",__FILE__,__LINE__,index_key.c_str());
    ldb_vec_scan(db, index_key, &ks);
    for(std::vector<std::string>::const_iterator it = ks.begin();
        it != ks.end();
        it++ ) {
            printf("vid:%s\n",it->c_str());
            Vertex v(*it, db, _gg_mtx);
            if(v.valid()) {
                vv.push_back(v);
            }
    }
    return vv;
}

std::vector<Edge> Graph::find_e(const std::string & val, const std::string & field) {
    std::vector<Edge> ev;
    std::string index_key(kIndexEdgePrefix);
    index_key += field;
    index_key += kPropGlue;
    index_key += gval_encode(val);
    index_key += kPropGlue;
    std::vector<std::string>  ks;
    //printf("[%s_%d]find e: %s\n",__FILE__,__LINE__,index_key.c_str());
    ldb_vec_scan(db,index_key,&ks);
    for(std::vector<std::string>::const_iterator it = ks.begin();
        it != ks.end();
        it++ ) {
            printf("ftid:%s\n",it->c_str());
            if(it->length() == kFtLen) {
                std::string from_id = it->substr(0,kOidLen);
                std::string to_id = it->substr(kOidLen,kOidLen);
                Edge e(from_id, to_id, db, _gg_mtx);
                if(e.valid()){
                    ev.push_back(e);
                }
            }else{
                printf("error len : %s %lu \n",it->c_str(), it->length());
            }
    }
    return ev;
}

void Graph::dump() {
    std::map<std::string,std::string> dumps;
    ldb_map_scan(db,"",&dumps);
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
