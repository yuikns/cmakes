#include "argcv/graph/edge.h"


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
#include "./ldb_def.h"

namespace argcv {
namespace graph {

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

const std::set<string> Edge::drop_set() const{
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



}  // namespace graph
}  // namespace argcv
