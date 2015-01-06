// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#ifndef LIB_ARGCV_GRAPH_G_UTILS_H_
#define LIB_ARGCV_GRAPH_G_UTILS_H_

#include <string>

#include "argcv/util/util.h"

namespace argcv {
namespace graph {

inline const std::string gval_encode(const std::string & ival) {
    //return ival;
    return std::to_string(argcv::util::BlzKeygen::instance().hash(ival,0)).append("#")
            .append(std::to_string(argcv::util::BlzKeygen::instance().hash(ival,1))).append("#")
                .append(std::to_string(argcv::util::BlzKeygen::instance().hash(ival,2)));
}

//inline const std::string gval_decode(const std::string & ival) {
//    return ival;
//}

}  // namespace graph
}  // namespace argcv

#endif  // LIB_ARGCV_GRAPH_G_UTILS_H_
