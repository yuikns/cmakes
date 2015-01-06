#include "argcv/graph/vertex.h"


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
#include "./ldb_def.h"
#include "./g_utils.h"

namespace argcv {
namespace graph {

// may optimiazed in the future
const std::vector<Vertex> Vertex::inV(const std::string & val, const std::string & field) const{
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
const std::vector<Vertex> Vertex::outV(const std::string & val, const std::string & field) const{
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

const std::vector<Edge> Vertex::inE(const std::string & val, const std::string & field) const{
    std::vector<Edge> ines;
    std::vector<std::string> ine_keys;
    std::string tmp_key(kIndexVertexInPrefix);
    tmp_key += _id;
    ldb_vec_scan(db, tmp_key, &ine_keys);
    if(val.length() == 0 ) {
        for(std::vector<std::string>::const_iterator it = ine_keys.begin();
                it != ine_keys.end();
                it ++) {
            //ines.push_back(Edge(_id, *it, db, _gg_mtx));
            ines.push_back(Edge(*it, _id, db, _gg_mtx));
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
                ines.push_back(Edge(*it, _id, db, _gg_mtx));
            }
        }
    }
    return ines;
}

const std::vector<Edge> Vertex::outE(const std::string & val, const std::string & field) const {
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
    std::vector<std::string> vkeys;
    ldb_vec_scan(db, kPropVertexPrefix + _id  , &vkeys);
    return vkeys;
}

bool Vertex::drop() {
    _gg_mtx->lock();
    bool status = ldb_batch_del(db, drop_set());
    _gg_mtx->unlock();
    return status;
}

const std::set<std::string> Vertex::drop_set() const {
    std::set<std::string> rm_keys;
    std::string tmp_key;
    std::vector<Edge> inEv = inE();
    std::vector<Edge> outEv = outE();
    
    tmp_key = kVertexPrefix;
    tmp_key += _id;
    rm_keys.insert(tmp_key);

    // edges 
    for(std::vector<Edge>::const_iterator it = inEv.begin();
            it != inEv.end();
            it ++ ) {
        const std::set<std::string> d_set(it->drop_set());
        rm_keys.insert(d_set.begin(),d_set.end());
    }
    for(std::vector<Edge>::const_iterator it = outEv.begin();
            it != outEv.end();
            it ++ ) {
        const std::set<std::string> d_set(it->drop_set());
        rm_keys.insert(d_set.begin(),d_set.end());
    }
    // indexes
    std::map<std::string,std::string> pro_map;
    tmp_key = kPropVertexPrefix;
    tmp_key += _id;

    ldb_map_scan(db, tmp_key , & pro_map);
    for(std::map<std::string,std::string>::const_iterator it= pro_map.begin();
            it!=pro_map.end();
            it++ ) {
        tmp_key = kPropVertexPrefix;
        tmp_key += _id;
        tmp_key += it->first;
        rm_keys.insert(tmp_key);

        tmp_key = kIndexVertexPrefix;
        tmp_key += it->first;
        tmp_key += kPropGlue;
        tmp_key += gval_encode(it->second);
        tmp_key += kPropGlue;
        tmp_key += _id;
        rm_keys.insert(tmp_key);
    }
    return rm_keys;
}

const std::string Vertex::get(const std::string & k) {  // get value by field
    std::string val;
    bool status = ldb_get(db, kPropVertexPrefix + _id + k, &val);
    return status ? val : "";
}

void Vertex::set(const std::string &k, const std::string &v, bool index) {  // set value by field
    std::string oval;
    std::string key(kPropVertexPrefix + _id + k);
    std::string index_key;
    _gg_mtx->lock();
    bool status = ldb_get(db,key, &oval);
    if (status) {
        index_key = kIndexVertexPrefix;
        index_key += k;
        index_key += kPropGlue;
        index_key += oval;
        index_key += kPropGlue;
        index_key += _id;
        ldb_del(db, index_key );
    }
    ldb_set(db,kPropVertexPrefix + _id + k, v);
    if (index) {
        index_key = kIndexVertexPrefix;
        index_key += k;
        index_key += kPropGlue;
        index_key += gval_encode(v);
        index_key += kPropGlue;
        index_key += _id;
        ldb_set(db, index_key);
    }
    _gg_mtx->unlock();
}

bool Vertex::valid(){
    return _valid && ldb_has(db, kVertexPrefix + _id);
}

}  // namespace graph
}  // namespace argcv

