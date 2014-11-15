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

namespace argcv {
namespace graph {

using std::string;
using std::vector;
using std::map;
using std::set;

const std::string kVertexPrefix = "V:";
const std::string kEdgePrefix = "E:";
const std::string kIndexVertexPrefix = "IV:";
const std::string kIndexEdgePrefix = "IE:";
const std::string kPropGlue = "$";

/////////////////////// Leveldb Utils ////////////////////////////////////

inline void ldb_map_scan(leveldb::DB* db, const std::string &base, std::map<std::string,std::string> * _kvs) {
    leveldb::ReadOptions snap_read_opt;
    snap_read_opt.snapshot = db->GetSnapshot();
    leveldb::Iterator* it = db->NewIterator(snap_read_opt);
    leveldb::Slice  slice= base;
    unsigned psz= base.size(); // prefix size
    // for (it->Seek(slice),it->Next(); // ignore first one ?
    for (it->Seek(slice);
            it->Valid() &&
                std::mismatch(base.begin(),base.end(),
                    it->key().ToString().begin()).first == base.end();
            it->Next()) {
        //std::cout << "found : " <<it->key().ToString().substr(psz,it->key().ToString().size()) << "\t"  << it->value().ToString() << std::endl;
        _kvs->insert(
            std::make_pair(
                it->key().ToString().substr(
                    psz,
                    it->key().ToString().size()),
                it->value().ToString()));
    }
    delete it;
    db->ReleaseSnapshot(snap_read_opt.snapshot);
}

inline void ldb_set_scan(leveldb::DB* db, const std::string &base, std::set<std::string> * _keys) {
    leveldb::ReadOptions snap_read_opt;
    snap_read_opt.snapshot = db->GetSnapshot();
    leveldb::Iterator* it = db->NewIterator(snap_read_opt);
    leveldb::Slice  slice= base;
    unsigned psz= base.size(); // prefix size
    // for (it->Seek(slice),it->Next(); // ignore first one ?
    for (it->Seek(slice);
            it->Valid() &&
                std::mismatch(base.begin(),base.end(),
                    it->key().ToString().begin()).first == base.end();
            it->Next()) {
        //std::cout << "found : " <<it->key().ToString().substr(psz,it->key().ToString().size()) << "\t"  << it->value().ToString() << std::endl;
        _keys->insert(
            it->key().ToString().substr(
                psz,
                it->key().ToString().size()));
    }
    delete it;
    db->ReleaseSnapshot(snap_read_opt.snapshot);
}

inline void ldb_vec_scan(leveldb::DB* db, const std::string &base, std::vector<std::string> * _keys) {
    leveldb::ReadOptions snap_read_opt;
    snap_read_opt.snapshot = db->GetSnapshot();
    leveldb::Iterator* it = db->NewIterator(snap_read_opt);
    leveldb::Slice  slice= base;
    unsigned psz= base.size(); // prefix size
    // for (it->Seek(slice),it->Next(); // ignore first one ?
    for (it->Seek(slice);
            it->Valid() &&
                std::mismatch(base.begin(),base.end(),
                    it->key().ToString().begin()).first == base.end();
            it->Next()) {
        //std::cout << "found : " <<it->key().ToString().substr(psz,it->key().ToString().size()) << "\t"  << it->value().ToString() << std::endl;
        _keys->push_back(
            it->key().ToString().substr(
                psz,
                it->key().ToString().size()));
    }
    delete it;
    db->ReleaseSnapshot(snap_read_opt.snapshot);
}

inline bool ldb_has(leveldb::DB* db, const std::string &key) {
	std::string val;
	return ((db->Get(leveldb::ReadOptions(),key, &val)).ok());
}

inline bool ldb_set(leveldb::DB* db, const std::string &key, const std::string &val = "") {
    return ((db->Put(leveldb::WriteOptions(),key,val)).ok());
}

inline bool ldb_get(leveldb::DB* db, const std::string &key, std::string *_val) {
    return ((db->Get(leveldb::ReadOptions(), key, _val)).ok());
}

inline bool ldb_get_one(leveldb::DB* db, const std::string &key, std::string *_val) {
    return ((db->Get(leveldb::ReadOptions(), key, _val)).ok());
}

inline bool ldb_del(leveldb::DB* db, const std::string &key) {
    return ((db->Delete(leveldb::WriteOptions(), key)).ok());
}

inline bool ldb_batch_add(leveldb::DB* db, const std::map<std::string,std::string> & kvs) {
    leveldb::WriteBatch * _wb = new leveldb::WriteBatch;
    for (std::map<std::string,std::string>::const_iterator it= kvs.begin();
            it!=kvs.end();
            it++ ) {
                printf("ldb_batch_add %s -> %s\n", it->first.c_str(), it->second.c_str());
        _wb->Put(it->first, it->second);
    }
    bool rst = ((db->Write(leveldb::WriteOptions(), _wb)).ok());
    delete _wb;
    return rst;
}

inline bool ldb_batch_del(leveldb::DB* db, const std::set<std::string> & keys) {
    leveldb::WriteBatch * _wb = new leveldb::WriteBatch;
    for (std::set<std::string,std::string>::const_iterator it= keys.begin();
            it!=keys.end();
            it++ ) {
        _wb->Delete(*it);
    }
    bool rst = ((db->Write(leveldb::WriteOptions(), _wb)).ok());
    delete _wb;
    return rst;
}
/////////////////////// Leveldb Utils End ////////////////////////////////

// V: <- Vertex
// E: <- Edge
// IV: <- Index Of Vertex
// IE: <- Index of Edge
// Edge
// TODO Edge Section
const std::vector<std::string> Edge::keys() const {
    _gg_mtx->lock();
    std::vector<std::string> ekeys;
    _gg_mtx->unlock();
    return ekeys;
}
bool Edge::drop() {
    return true;
}

const std::string Edge::get(const std::string & k) {
    return "ni";
}

void Edge::set(const std::string &k, const std::string &v, bool index) {
}

// Vertex
// TODO Vertex Section
const std::vector<Vertex> Vertex::inV(const std::string & val, const std::string & field) {
    std::vector<Vertex> invs;

    return invs;
}

const std::vector<Vertex> Vertex::outV(const std::string & val, const std::string & field) {
    std::vector<Vertex> outvs;

    return outvs;
}

const std::vector<Edge> Vertex::inE(const std::string & val, const std::string & field) {
    std::vector<Edge> ines;

    return ines;
}

const std::vector<Edge> Vertex::outE(const std::string & val, const std::string & field) {
    std::vector<Edge> outes;

    return outes;
}

const std::vector<std::string> Vertex::keys() const {
    std::vector<std::string> keys;

    return keys;
}

bool Vertex::drop() {
    _gg_mtx->lock();
    
    _gg_mtx->unlock();
    return true;
}

const std::string Vertex::get(const std::string & k) {  // get value by field
    return "";
}

void Vertex::set(const std::string &k, const std::string &v, bool index) {  // set value by field

}

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
    return Vertex();
}
Vertex Graph::id_v(const std::string &id) {
    return Vertex();
}

Edge Graph::id_e(const std::string &id) {
    return Edge();
}

Vertex Graph::find_v(const std::string & val, const std::string & field) {
    return Vertex();
}

Edge Graph::find_e(const std::string & val, const std::string & field) {
    return Edge();
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
