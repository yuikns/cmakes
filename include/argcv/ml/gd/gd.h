// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#ifndef INCLUDE_ARGCV_ML_GD_GD_H_
#define INCLUDE_ARGCV_ML_GD_GD_H_

#include <vector>

#include "argcv/ml/mldef.h"

namespace argcv {
namespace ml {
namespace gd {

// dataset , learn rate , lose control ,
// weights , max step , dynamic learn rate ?
double sgd(DDDataSet ds, double delta,
        double epsilon, std::vector<double> * _w,
        uint64_t step_limit = 1000, bool dynamd = true);
double bgd(DDDataSet ds, double delta,
        double epsilon, std::vector<double> * _w,
        uint64_t step_limit = 1000, bool dynamd = true);

inline double gd(DDDataSet ds, double delta,
        double epsilon, std::vector<double> * _w,
        uint64_t step_limit = 1000, bool dynamd = true) {
    return sgd(ds, delta, epsilon, _w, dynamd);
}

}  // namespace gd
}  // namespace ml
}  // namespace argcv

#endif  // INCLUDE_ARGCV_ML_GD_GD_H_
