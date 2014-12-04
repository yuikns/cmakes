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


namespace argcv {
namespace graph {


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
    std::vector<std::string> vkeys;
    ldb_vec_scan(db, kPropVertexPrefix + _id  , &vkeys);
    return vkeys;
}

//TODO
bool Vertex::drop() {
    _gg_mtx->lock();
    bool status = ldb_batch_del(db, drop_set());
    _gg_mtx->unlock();
    return status;
}


/*
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
*/
std::set<std::string> Vertex::drop_set() {
    std::set<std::string> rm_keys;
    std::vector<Edge> inEv = inE();
    std::vector<Edge> outEv = outE();
    for(std::vector<Edge>::const_iterator it = inEv.begin();
            it != inEv.end();
            it ++ ) {
        
    }
    return rm_keys;
}

const std::string Vertex::get(const std::string & k) {  // get value by field
    return "";
}

void Vertex::set(const std::string &k, const std::string &v, bool index) {  // set value by field

}

}  // namespace graph
}  // namespace argcv

