// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#include <stdio.h>

#include <map>

#include "argcv/graph/graph.h"

using argcv::graph::Graph;

int main(int argc, char * argv[]) {
    Graph g("graph.db");
    g.test();
    g.dump();
    return 0;
}




