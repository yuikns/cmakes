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
const std::string kInGlue = "IN:";
const std::string kOutGlue = "OUT:";

const std::string kPropVertexPrefix = kPropGlue + kVertexPrefix;
const std::string kPropEdgePrefix = kPropGlue + kEdgePrefix;
const std::string kIndexVertexInPrefix = kInGlue + kIndexVertexPrefix;
const std::string kIndexVertexOutPrefix = kOutGlue + kIndexVertexPrefix;
// const std::string kIndexEdgeInPrefix = kInGlue + kIndexEdgePrefix;
// const std::string kIndexEdgeOutPrefix = kOutGlue + kIndexEdgePrefix;

/////////////////////// Leveldb Utils ////////////////////////////////////

inline void ldb_map_scan(leveldb::DB* db, const std::string &base, 
        std::map<std::string,std::string> * _kvs) {
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

// warning .. low performance , waiting ..
inline void ldb_map_scan_ext(leveldb::DB* db, const std::string &base, 
        std::map<std::string,std::string> * _kvs,
        const std::string & k_prefix = "", const std::string & k_suffix = "",
        const std::string & v_prefix = "", const std::string & v_suffix = "") {
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
                k_prefix +
                    it->key().ToString().substr(
                        psz,
                        it->key().ToString().size()) +
                    k_suffix,
                v_prefix +
                    it->value().ToString() +
                    v_suffix));
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

// warning .. low performance , waiting ..
inline void ldb_set_scan_ext(leveldb::DB* db, const std::string &base, std::set<std::string> * _keys,
        const std::string & prefix = "", const std::string & suffix = "") {
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
            prefix + 
            it->key().ToString().substr(
                psz,
                it->key().ToString().size()) +
            suffix);
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

// warning .. low performance , waiting ..
inline void ldb_vec_scan_ext(leveldb::DB* db, const std::string &base, std::vector<std::string> * _keys,
        const std::string & prefix = "", const std::string & suffix = "") {
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
            prefix + 
            it->key().ToString().substr(
                psz,
                it->key().ToString().size()) +
            suffix);
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

inline bool ldb_get_one(leveldb::DB* db, const std::string &key, std::string *_f_key , std::string * _val) {
    leveldb::Iterator* it = db->NewIterator(leveldb::ReadOptions());
    leveldb::Slice  slice= key;
    // for (it->Seek(slice),it->Next(); // ignore first one ?
    it->Seek(slice);
    if (it->Valid() &&
            std::mismatch(key.begin(),key.end(),
            it->key().ToString().begin()).first == key.end()) {
        if(_f_key != NULL) {
            _f_key->assign(it->key().ToString().substr(
                        key.size(),
                        it->key().ToString().size()));
        }
        if(_val != NULL) {
            _val->assign(it->value().ToString());
        }
        return true;
    }else {
        return false;
    }
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
    std::vector<std::string> ekeys;
    ldb_vec_scan(db, kPropEdgePrefix + ft_id , &ekeys);
    return ekeys;
}
bool Edge::drop() {
    _gg_mtx->lock();
    bool status = ldb_batch_del(db, drop_set());
    _gg_mtx->unlock();
    return status;
}

std::set<string> Edge::drop_set() {
    std::set<std::string> rm_keys;
    std::string tmp_key;
    tmp_key = kEdgePrefix;
    tmp_key += ft_id;
    rm_keys.insert(tmp_key);
    tmp_key = kIndexVertexOutPrefix;
    tmp_key += from_id;
    rm_keys.insert(tmp_key); // source out
    tmp_key = kIndexVertexInPrefix;
    tmp_key += to_id;
    rm_keys.insert(tmp_key); // dest in
    std::map<std::string,std::string> pro_map;
    tmp_key = kPropEdgePrefix;
    tmp_key += ft_id;
    ldb_map_scan(db, tmp_key , & pro_map);
    
    for(std::map<std::string,std::string>::const_iterator it= pro_map.begin();
            it!=pro_map.end();
            it++ ) {
        tmp_key = kPropEdgePrefix;
        tmp_key += ft_id;
        tmp_key += it->first;
        rm_keys.insert(tmp_key);

        tmp_key = kIndexEdgePrefix;
        tmp_key += it->first;
        tmp_key += kPropGlue;
        tmp_key += it->second;
        tmp_key += kPropGlue;
        tmp_key += ft_id;
        rm_keys.insert(tmp_key);
    }
    for(std::set<std::string>::const_iterator it = rm_keys.begin();
            it != rm_keys.end();
            it++ ) {
        printf("Edge::drop_set %s\n",it->c_str());
    }
    return rm_keys;
}


const std::string Edge::get(const std::string & k) {
    std::string val;
    bool status = ldb_get(db, kPropEdgePrefix + ft_id + k, &val);
    return status ? val : "";
}

void Edge::set(const std::string &k, const std::string &v, bool index) {
    std::string oval;
    std::string key(kPropEdgePrefix + ft_id + k);
    std::string index_key;
    _gg_mtx->lock();
    bool status = ldb_get(db,key, &oval);
    if (status) {
        index_key = kIndexEdgePrefix;
        index_key += k;
        index_key += kPropGlue;
        index_key += oval;
        index_key += kPropGlue;
        index_key += ft_id;
        ldb_del(db, index_key );
    }
    ldb_set(db,kPropEdgePrefix + ft_id + k, v);
    if (index) {
        index_key = kIndexEdgePrefix;
        index_key += k;
        index_key += kPropGlue;
        index_key += v;
        index_key += kPropGlue;
        index_key += ft_id;
        ldb_set(db, index_key);
    }
    _gg_mtx->unlock();
}

// Vertex
// may optimiazed in the future
const std::vector<Vertex> Vertex::inV(const std::string & val, const std::string & field) {
    std::vector<Vertex> invs;
    std::vector<std::string> inv_keys;
    std::string tmp_key(kIndexVertexInPrefix);
    tmp_key += _id;
    ldb_vec_scan(db, tmp_key, &inv_keys);
    if(val.length() == 0 ) {
        for(std::vector<std::string>::const_iterator it = inv_keys.begin();
                it != inv_keys.end();
                it ++) {
            invs.push_back(Vertex(*it, db, _gg_mtx));
        }
    } else {
        for(std::vector<std::string>::const_iterator it = inv_keys.begin();
                it != inv_keys.end();
                it ++) {
            std::string prop;
            std::string qkey = kPropVertexPrefix;
            qkey += *it;
            qkey += field;
            if(ldb_get(db, qkey, &prop) && val.compare(prop) == 0) {
                invs.push_back(Vertex(*it, db, _gg_mtx));
            }
        }
    }
    return invs;
}

// may optimiazed in the future
const std::vector<Vertex> Vertex::outV(const std::string & val, const std::string & field) {
    std::vector<Vertex> outvs;
    std::vector<std::string> outv_keys;
    std::string tmp_key(kIndexVertexOutPrefix);
    tmp_key += _id;
    ldb_vec_scan(db, tmp_key, &outv_keys);
    if(val.length() == 0 ) {
        for(std::vector<std::string>::const_iterator it = outv_keys.begin();
                it != outv_keys.end();
                it ++) {
            outvs.push_back(Vertex(*it, db, _gg_mtx));
        }
    } else {
        for(std::vector<std::string>::const_iterator it = outv_keys.begin();
                it != outv_keys.end();
                it ++) {
            std::string prop;
            std::string qkey = kPropVertexPrefix;
            qkey += *it;
            qkey += field;
            if(ldb_get(db, qkey, &prop) && val.compare(prop) == 0) {
                outvs.push_back(Vertex(*it, db, _gg_mtx));
            }
        }
    }
    return outvs;
}

const std::vector<Edge> Vertex::inE(const std::string & val, const std::string & field) {
    std::vector<Edge> ines;
    std::vector<std::string> ine_keys;
    std::string tmp_key(kIndexVertexInPrefix);
    tmp_key += _id;
    ldb_vec_scan(db, tmp_key, &ine_keys);
    if(val.length() == 0 ) {
        for(std::vector<std::string>::const_iterator it = ine_keys.begin();
                it != ine_keys.end();
                it ++) {
            ines.push_back(Edge(_id, *it, db, _gg_mtx));
        }
    } else {
        for(std::vector<std::string>::const_iterator it = ine_keys.begin();
                it != ine_keys.end();
                it ++) {
            std::string prop;
            std::string qkey = kPropEdgePrefix;
            qkey += _id;
            qkey += *it;
            qkey += field;
            if(ldb_get(db, qkey, &prop) && val.compare(prop) == 0) {
                ines.push_back(Edge(_id, *it, db, _gg_mtx));
            }
        }
    }
    return ines;
}

const std::vector<Edge> Vertex::outE(const std::string & val, const std::string & field) {
    std::vector<Edge> outes;
    std::vector<std::string> oute_keys;
    std::string tmp_key(kIndexVertexOutPrefix);
    tmp_key += _id;
    ldb_vec_scan(db, tmp_key, &oute_keys);
    if(val.length() == 0 ) {
        for(std::vector<std::string>::const_iterator it = oute_keys.begin();
                it != oute_keys.end();
                it ++) {
            outes.push_back(Edge(_id, *it, db, _gg_mtx));
        }
    } else {
        for(std::vector<std::string>::const_iterator it = oute_keys.begin();
                it != oute_keys.end();
                it ++) {
            std::string prop;
            std::string qkey = kPropEdgePrefix;
            qkey += _id;
            qkey += *it;
            qkey += field;
            if(ldb_get(db, qkey, &prop) && val.compare(prop) == 0) {
                outes.push_back(Edge(_id, *it, db, _gg_mtx));
            }
        }
    }
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

std::set<std::string> Vertex::drop_set() {
    std::set<std::string> rm_keys;
    return rm_keys;
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
