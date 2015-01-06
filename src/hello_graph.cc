// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#include <stdio.h>

#include <map>

#include <chrono>

#include <stdlib.h>

#include "argcv/graph/graph.h"
#include "argcv/util/timer.h"

using argcv::graph::Graph;
using namespace argcv::graph;
using argcv::util::Timer;

int main(int argc, char * argv[]) {
    //typedef std::chrono::high_resolution_clock hrclock;
    //typedef std::chrono::duration<double, std::milli> mil;
    //typedef std::chrono::duration<int64_t, std::ratio<1,1000000000>> nal;
    //typedef std::chrono::time_point<hrclock, nal> time_point;
    Graph g("graph.db");
    //time_point  t0 = hrclock::now();
    Timer t;
    Vertex a = g.new_v();
    a["name"] = "VNAME_A";
    printf("aid: %s , len: %lu\n",a.id().c_str(),a.id().length());
    Vertex b = g.new_v();
    b["name"] = "VNAME_B";
    printf("bid: %s , len: %lu\n",b.id().c_str(),b.id().length());
    Vertex c = g.new_v();
    c["name"] = "VNAME_C";
    printf("cid: %s , len: %lu\n",b.id().c_str(),b.id().length());
    
    Vertex ac = g.id_v(a.id());
    ac["flag"] = "set flag by ac";
    
    auto ks = ac.keys();
    for(auto it = ks.begin() ; it != ks.end() ; it ++ ) {
        printf("key : %s val : %s\n",(*it).c_str(), ((std::string) a[*it]).c_str());
    }
    
    printf("a val : %s\n",((std::string) a["flag"]).c_str());
    
    
    g.link(a.id(),b.id())["label"]="edge";
    g.link(a.id(),c.id())["label"]="edge";
    g.link(b.id(),c.id())["label"]="edge";
    g.link(c.id(),a.id())["label"]="edge";
    
    auto aov = a.outV();
    for(auto it = aov.begin() ; it != aov.end() ; it ++ ) {
        printf("V<aov> is valid : %c \n", it->valid() ? 'Y':'N');
        //(*it)["label"]="edge";
        printf("aov id %s \n", it->id().c_str() );
    }
    
    auto biv = b.inV();
    for(auto it = biv.begin() ; it != biv.end() ; it ++ ) {
        printf("E<biv> is valid : %c \n", it->valid() ? 'Y':'N');
        //(*it)["label"]="edge";
        //printf("biv id %s \n", it->id().c_str() );
    }

    printf("[Vertex Find Size] %lu\n",g.find_v("VNAME_A").size());
    printf("[Find V] vertex , a : %s %s\n",g.find_one_v("VNAME_A").id().c_str(), ((std::string)g.find_one_v("VNAME_A")["name"]).c_str());
    
    auto es = g.find_e("edge","label");
    printf("[Edge Find Size] %lu\n",es.size());
    for(auto it = es.begin() ; it != es.end() ; it ++ ) {
        printf("E<biv> is valid : %c \n", it->valid() ? 'Y':'N');
        //printf("[Find Edge] F: %s , T: %s, FT: %s \n", it->from().c_str(), it->to().c_str(), it->ft().c_str() );
        printf("[LINK] %s -> %s \n",
                ((std::string)g.id_v(it->from())["name"]).c_str(),
                ((std::string)g.id_v(it->to())["name"]).c_str());
    }

    t.label("t1");
    //time_point  t1 = hrclock::now();
    //g.test();

    //a.drop();
    //b.drop();
    //c.drop();
    printf("~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~DUMP\n");
    g.dump();
    //time_point  t2 = hrclock::now();
    t.label("t2");
    printf("time cost : all : %f ms, t1->t2 : %f ms, from t1 : %f ms, from t2 : %f ms, to t1: %f ms, to t2: %f ms\n",
        t.all(),
        t.between("t1","t2"),
        t.from("t1"),
        t.from("t2"),
        t.to("t1"),
        t.to("t2"));
    return 0;
}




