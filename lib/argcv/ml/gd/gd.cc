// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#include "argcv/ml/gd/gd.h"

#include <stdio.h>

#include <vector>

#include "argcv/ml/mldef.h"

namespace argcv {
namespace ml {
namespace gd {
using std::vector;

// return x^v
inline double nsq(double x, double v) {
    double r = 1;
    for (int i = 0; i < v; i++) {
        r *= x;
    }
    return r;
}

// gd fun
inline double f(std::vector<double> x, const std::vector<double> &w) {
    double fx = w[0];
    for (unsigned i = 0; i < x.size(); i ++)
        fx += x[i] * w[i+1];
    return fx;
}

// lose
inline double l(std::vector<double> x, double y, const std::vector<double> &w) {
    double fx = f(x, w);
    return (fx - y) * (fx - y) / 2;
}

inline double l(std::pair <std::vector<double>, double> ds_item,
        const std::vector<double> &w) {
    return l(ds_item.first, ds_item.second, w);
}

// lose check
inline double lc(DDDataSet ds, const std::vector<double> &w) {
    double sum = 0;
    for (unsigned i = 0; i < ds.size(); i++) {
        sum += l(ds.Xat(i), ds.Yat(i), w);
    }
    return sum/ds.size();
}


double sgd(DDDataSet ds, double delta,
        double epsilon, std::vector<double> * _w,
        uint64_t step_limit, bool dynamd) {
    double m_ls;
    if (ds.size() < 1) {
        return -1;
    }
    vector<double> &w = *_w;
    // w.assign(0, ds.size() + 1);
    // w.clear();
    // w.push_back(0);
    // for(unsigned i = 0 ; i < ds.Xat(0).size() ; i++ ) {
    //     w.push_back(0);
    // }
    w.assign(ds.Xat(0).size() + 1, 0);

    // printf("w size:%lu\n", w.size());

    uint64_t c_step = 0;
    while ((m_ls = lc(ds, w)) > epsilon && c_step < step_limit) {
        c_step++;
        // show current status
        if (c_step % 1000 == 0) {
            printf("%llu\t%f\t", c_step, m_ls);
            for (unsigned i = 0 ; i < w.size() ; i++) {
                printf("w%u=%f\t", i, w[i]);
            }
            printf("\n");
        }
        for (unsigned i = 0; i < ds.size(); i++) {
            double fx = f(ds.Xat(i), w);
            for (int j = 0; j < w.size(); j++) {
                w[j] +=  delta * (ds.Yat(i) - fx)
                    * (j > 0 ? ds.Xat(i, j-1) : 1);
            }
        }
    }
    return m_ls;
}

double bgd(DDDataSet ds, double delta,
        double epsilon, std::vector<double> * _w,
        uint64_t step_limit, bool dynamd) {
    // return 999.9999;  // not implemented
    // ref : https://github.com/yujing5b5d/GradientDescent/blob/master/main.cc
    return sgd(ds, delta, epsilon, _w, step_limit, dynamd);
}

}  // namespace gd
}  // namespace ml
}  // namespace argcv
