// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#include <stdio.h>

#include <map>

#include <chrono>

#include <stdlib.h>

#include "argcv/graph/graph.h"

using argcv::graph::Graph;

int main(int argc, char * argv[]) {
    typedef std::chrono::high_resolution_clock hrclock;
    typedef std::chrono::duration<double, std::milli> mil;
    typedef std::chrono::duration<int64_t, std::ratio<1,1000000000>> nal;
    typedef std::chrono::time_point<hrclock, nal> time_point;
    Graph g("graph.db");
    time_point  t0 = hrclock::now();
    g.test();
    g.dump();
    time_point  t1 = hrclock::now();
    system("sleep 1");
    time_point  t2 = hrclock::now();
    printf("time cost : %f + %f ms\n",mil(t1-t0).count(), mil(t2-t1).count());
    return 0;
}




