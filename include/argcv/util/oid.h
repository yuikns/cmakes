// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#ifndef INCLUDE_ARGCV_UTIL_OID_H_
#define INCLUDE_ARGCV_UTIL_OID_H_

#include <stdint.h>  // uint64_t
#include <time.h>

#include <string>
#include <mutex>  // for std::lock_guard

namespace argcv {
namespace util {
// object id generator
class ObjId {
 public :
    static ObjId & instance() {
        static ObjId oid;
        return oid;
    }
    virtual ~ObjId();
    const std::string id();

 private :
    ObjId();  // Private constructor
    ObjId(const ObjId &);  // Prevent copy-construction
    ObjId&  operator=(const ObjId &);  // Prevent assignment
    void seed(time_t * _sec, uint16_t * _seq);
    uint16_t seq;
    uint32_t pid;
    time_t cnt_time;
    time_t t_seed;
    std::mutex m_mutex;  // by <mutex> for lock_guard
};

}  // namespace util
}  // namespace argcv

#endif  // INCLUDE_ARGCV_UTIL_OID_H_
