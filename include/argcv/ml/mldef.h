// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#ifndef INCLUDE_ARGCV_ML_MLDEF_H_
#define INCLUDE_ARGCV_ML_MLDEF_H_

#include <stdint.h>  // uint64_t

#include <string>
#include <vector>
#include <utility>  // std::pair, std::make_pair

namespace argcv {
namespace ml {

template <typename X, typename Y>
class DataSet {
 public:
    DataSet() {
    }

    std::vector< std::pair <std::vector<X>, Y> > data() {
        return ds;
    }

    virtual ~DataSet() {
    }

    uint64_t size() {
        return ds.size();
    }

    std::pair <std::vector<X>, Y> & operator[](uint64_t pos) {
        return ds[pos];
    }

    std::pair <std::vector<X>, Y> & at(uint64_t pos) {
        return ds.at(pos);
    }

    std::vector<X> & Xat(uint64_t pos) {
        return at(pos).first;
    }

    X & Xat(uint64_t pos, uint64_t off) {
        return Xat(pos).at(off);
    }

    Y & Yat(uint64_t pos) {
        return at(pos).second;
    }

    void add(std::pair <std::vector<X>, Y> ds_item) {
        return ds.push_back(ds_item);
    }

    void add(std::vector<X> x, Y y) {
        std::pair<std::vector<X>, Y> val(x, y);
        return ds.push_back(val);
    }

    void add(X x[], int len, Y y) {
        std::pair<std::vector<X>, Y> val(
            std::vector<X>(x, x + len), y);
        return ds.push_back(val);
    }

    void rm(uint64_t pos) {
        ds.erase(ds.begin() + pos);
    }

    void rm() {
        ds.clear();
    }

 private:
    std::vector< std::pair <std::vector<X>, Y> > ds;
};

typedef DataSet<double, int> DDataSet;
typedef DataSet<double, bool> DBDataSet;
typedef DataSet<double, double> DDDataSet;
typedef DataSet<std::string, int> SDataSet;
typedef DataSet<std::string, bool> SBDataSet;
typedef DataSet<std::string, std::string> SSDataSet;

}  // namespace ml
}  // namespace argcv

#endif  // INCLUDE_ARGCV_ML_MLDEF_H_
