#include "mem/cache/countmin.hh"

#include <cmath>
#include <ctime>
#include <iostream>

CountMinCounter::CountMinCounter(float eps, float gamma){
    width = ceil(exp(1)/eps);
    depth = ceil(log(1/gamma));
    std::cout << "CountMinCounter with " << eps << " " << gamma << " " << width << " " << depth << std::endl;
    auto C = new unsigned int *[depth];
    for (int i = 0; i < depth; i++){
        C[i] = new unsigned int[width];
        for (int j = 0; j < width; j++){
            C[i][j] = 0;
        }
    }
    counters = C;

    std::srand(time(nullptr));

    hashes = new unsigned int*[depth];
    for (int i = 0; i < depth; i++){
        hashes[i] = new unsigned int[2];
        hashes[i][0] = int(float(std::rand()) * float(LONG_PRIM)/float(RAND_MAX) + 1);
        hashes[i][1] = int(float(std::rand()) * float(LONG_PRIM)/float(RAND_MAX) + 1);
    }
};

void CountMinCounter::increment(int s){
    CountMinCounter::increment(s, 1);
}

void CountMinCounter::increment(char *s){
    CountMinCounter::increment(s, 1);
}

void CountMinCounter::increment(char *s, int update) {
    int strHash = hashstr(s);
    CountMinCounter::increment(strHash, update);
}

void CountMinCounter::increment(int s, int update) {
    int hashval;
    int strHash = s;
    for (int i = 0; i < depth; i++){
       hashval = (int((long)hashes[i][0]*strHash+hashes[i][1]%LONG_PRIM) % width);
       int hash = counters[i][hashval];
       hash += update;
       counters[i][hashval] = hash;
    }
}

int CountMinCounter::estimate(char *s) {
    int strHash = hashstr(s);
    return CountMinCounter::estimate(strHash);
}

int CountMinCounter::estimate(int s) {
    int hashval;
    int strHash = s;
    int estimate = -1;
    for (int i = 0; i < depth; i++){
        hashval = (int((long)hashes[i][0]*strHash+hashes[i][1]%LONG_PRIM) % width);
        int hash = counters[i][hashval];
        if (hash < estimate || estimate < 0){
            estimate = hash;
        }
    }
    return estimate;
}

int CountMinCounter::hashstr(char *s){
    long hash = 2540;
    int c;
    c = *s++;
    while (c){
        //cout << c << " ";
        hash= (hash) + hash + c;
        c = *s++;
    }
    //cout << endl;
    return hash;
}

void CountMinCounter::print() {
    cout << "Printing Data Structure" << endl;
    for (int i = 0; i < depth; i++){
        for (int j = 0; j < width; j++){
            cout << counters[i][j] << " ";
        }
        cout << endl;
    }
}

