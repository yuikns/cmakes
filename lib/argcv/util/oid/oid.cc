// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#include "argcv/util/oid.h"

#include <stdint.h>  // uint64_t
#include <time.h>  // for time generation
#include <unistd.h>  // for getpid() , get process id , getppid() , get parent id
// #include <stdio.h>

#include <string>
#include <mutex>  // for std::lock_guard

#include "argcv/util/util.h"

namespace argcv {
namespace util {

ObjId::ObjId() {
    t_seed = time(NULL);
    // cnt_time = t_seed;
    seq = 0;
    pid = getpid();
}

ObjId::~ObjId() {
}


const std::string ObjId::id() {
    time_t m_time = 0;
    uint16_t m_seq = 0;
    seed(&m_time, &m_seq);
    unsigned int eax = 1, ebx = 0, ecx = 0, edx = 0;
    asm volatile("cpuid"
          : "=a" (eax),
            "=b" (ebx),
            "=c" (ecx),
            "=d" (edx)
          : "0" (eax), "2" (ecx));
    char buff[81];
    buff[80] = '\0';
    snprintf(buff,
            80,
            "%08lX:%08X-%08X-%08X-%08X:%08lX#%08X:%016u",
            t_seed,
            eax,
            ebx,
            ecx,
            edx,
            m_time,
            m_seq,
            pid);
    // printf("val: %s \n", buff);
    char m_id[49];
    m_id[48] = '\0';
    BlzKeygen & k = BlzKeygen::instance();
    snprintf(m_id,
            48,
            "%016llx%016llx%016llx",
            k.hash(buff, 0),
            k.hash(buff, 1),
            k.hash(buff, 2));
    return std::string(m_id);
}

void ObjId::seed(time_t * _sec, uint16_t * _seq) {
    std::lock_guard<std::mutex> l(m_mutex);
    *_sec = time(NULL);
    if (*_sec == cnt_time) {
        // printf("EQ: %08lx - %08lx\n", *_sec, cnt_time);
        *_seq = ++seq;
    } else {
        // printf("NQ: %08lx - %08lx\n", *_sec, cnt_time);
        *_seq = seq = 0;
        cnt_time = *_sec;
    }
    // printf("RS: %08lx - %08lx\n", *_sec, cnt_time);
}


}  // namespace util
}  // namespace argcv

