// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#include <stdio.h>
#include <stdlib.h>

#include <vector>
#include <string>

#include "argcv/ml/mldef.h"
#include "argcv/ml/gd/gd.h"
#include "argcv/util.h"

// using argcv::ml::DataSet;
// using argcv::ml::DDataSet;
// using argcv::ml::SDataSet;
using argcv::ml::DDDataSet;
using std::vector;

using argcv::util::split;
using argcv::ml::gd::sgd;


int main(int argc, char * argv[]) {
    DDDataSet dd;
    char buff[256];
    FILE * f = fopen("../data/label0.dat","r");
    while(fgets(buff,256,f)) {
        vector<std::string> s = split(buff,",");
        if(s.size() == 2) {
            double x[2];
            // printf("len:%lu\n",s.size());
            x[0] = atof(s[0].c_str());
            x[1] = atof(s[1].c_str());
            // printf("ds: %f == %f \n",x[0],y);
            dd.add(x,2,0);
        } else {
            printf("not found : %s \n",buff);
        }
    }
    fclose(f);
    f = fopen("../data/label1.dat","r");
    while(fgets(buff,256,f)) {
        vector<std::string> s = split(buff,",");
        if(s.size() == 2) {
            double x[2];
            // printf("len:%lu\n",s.size());
            x[0] = atof(s[0].c_str());
            x[1] = atof(s[1].c_str());
            // printf("ds: %f == %f \n",x[0],y);
            dd.add(x,2,1);
        } else {
            printf("not found : %s \n",buff);
        }
    }
    fclose(f);
    
    printf("all loaded , size : %llu \n",dd.size());
    vector<double> w;
    sgd(dd,0.0000001,0.0001,&w,10000);
    for(unsigned i = 0 ; i < w.size() ; i++) {
        printf("%u : %f\n",i,w[i]);
    }
    return 0;
}

