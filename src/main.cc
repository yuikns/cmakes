// Copyright 2014 Yu Jing<yujing5b5d@gmail.com>
#include <cstdio>
// #include <iostream>
// #include "crfpp.h" // crfpp
#include "./config.h"
#include "argcv/util.h"
#include "argcv/util/dict.h"

#include "inih/INIReader.h"
// #include <cassert>


int main(int argc, char *argv[]) {
    printf("cmake sample starting ... in  version : %d.%.2f , "
            "compiled by  %s @ %s (%s) %s \n",
                GLO_VERSION_MAJOR,
                GLO_VERSION_MINOR,
                CMAKE_CXX_COMPILER_ID,
                CMAKE_SYSTEM,
                CMAKE_SYSTEM_NAME,
                CMAKE_SYSTEM_PROCESSOR);

    INIReader reader("config.ini");

    if (reader.ParseError() < 0) {
        fprintf(stderr, "Can't load 'config.ini'\n");
        return -1;
    }

    printf("dict_dir : %s , node : %ld \n",
        reader.Get("base", "dict_dir", "unknown?").c_str(),
        reader.GetInteger("base", "node", -1));

    printf("loading dict \n");

    argcv::util::Dict &d = argcv::util::Dict::instance();

    // also built
    // if you would like to build yourself , git submodule update first plz ,
    // and it is hard coded as "../res/webdict/webdict_with_freq.txt"
    // d.build("../res/webdict/webdict_with_freq.txt");

    printf("loaded \n");
    char buff[100];

    printf("input a chinese phrase , it will return a fequency .. \n");
    while ( scanf("%s", buff)  >= 1 ) {
        printf("%s %llu ~ %d \n", buff , d[buff], d.level(buff));
        printf("input a chinese phrase , it will return a fequency .. \n");
    }
    return 0;
}

