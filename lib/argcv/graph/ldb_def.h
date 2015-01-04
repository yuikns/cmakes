// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#ifndef LIB_ARGCV_GRAPH_LDB_DEF_H_
#define LIB_ARGCV_GRAPH_LDB_DEF_H_

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

const unsigned kOidLen = 47;
const unsigned kFtLen = 47 * 2;

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
        //std::cout << "found : " <<it->key().ToString().substr(psz,it->key().ToString().size())
                // << "\t"  << it->value().ToString() << std::endl;
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
        //printf("ldb_batch_add %s -> %s\n", it->first.c_str(), it->second.c_str());
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

}  // namespace graph
}  // namespace argcv

#endif  // LIB_ARGCV_GRAPH_LDB_DEF_H_

