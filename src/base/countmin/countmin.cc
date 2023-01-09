#include "base/countmin/countmin.hh"

#include <cmath>
#include <ctime>
#include <iostream>

CountMinCounter::CountMinCounter(float eps, float gamma){
    width = ceil(exp(1)/eps);
    depth = ceil(log(1/gamma));
    width = 16;
    depth = 6;
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
    std::random_device rd;
    std::mt19937_64 gen(rd());

    int rand_start = 0;
    int rand_end = 1000;
    hashes = new unsigned int*[depth];
    for (int i = 0; i < depth; i++){
        rand_start = i * 1000000;
        rand_end = rand_start + (i + 1) * 1000000 - 1;
        //cout << a << " " << b << endl;
        std::uniform_int_distribution<> distr(rand_start, rand_end);
        hashes[i] = new unsigned int[2];
        hashes[i][0] = int(float(std::rand()) * float(LONG_PRIM)/float(RAND_MAX) + 1);
        hashes[i][1] = int(float(std::rand()) * float(LONG_PRIM)/float(RAND_MAX) + 1);
        //hashes[i][0] = (int(float((std::rand()) * float(LONG_PRIM + std::rand())/float(RAND_MAX)) / float(((i + 1) * 1000) * depth * depth))) ;
        //hashes[i][1] = (int(float((std::rand()) * float(LONG_PRIM + std::rand())/float(RAND_MAX)) / float(((i + 1) * 1000) * depth * depth))) ;
        //hashes[i][0] = distr(gen);
        //hashes[i][1] = distr(gen);
        cout << "hashes for " << i << ": " << hashes[i][0] << " " << hashes[i][1] << endl;
    }

    hash_string_prime = (unsigned int *) &prime_array;
};

int CountMinCounter::increment(int s){
   return CountMinCounter::increment(s, 1);
}

int CountMinCounter::increment(char *s){
   return  CountMinCounter::increment(s, 1);
}

int CountMinCounter::increment(char *s, int update) {
//    cout << "CountMinKey: " << s << endl;
    unsigned long strHash = hashstr(s);
    return CountMinCounter::increment(strHash, update);
}

int CountMinCounter::increment(int s, int update) {
    return CountMinCounter::increment((unsigned long) s, update);
}

int CountMinCounter::increment(unsigned long s, int update){
//    cout << "CountMinKeyInt: " << s << endl;
    int hashval;
    unsigned long strHash = s;
    int estimate = -1;
    for (int i = 0; i < depth; i++) {
        //hashval = (int((long) hashes[i][0] * strHash + hashes[i][1] % LONG_PRIM) % width);
        hashval = (hashes[i][0] * strHash * hashes[i][1] >> 26) % width;
//        cout << "Index: " << i << " " << hashval << endl;
        int hash = counters[i][hashval];
        hash += update;
        counters[i][hashval] = hash;
        if (hash < estimate || estimate < 0) {
            estimate = hash;
        }
    }
//    cout << "Estimate: " << estimate << endl;
    return estimate;
}

int CountMinCounter::decrement(int s){
    return CountMinCounter::decrement(s, 1);
}

int CountMinCounter::decrement(char *s){
    return  CountMinCounter::decrement(s, 1);
}

int CountMinCounter::decrement(char *s, int update) {
//    cout << "CountMinKey: " << s << endl;
    unsigned long strHash = hashstr(s);
    return CountMinCounter::decrement(strHash, update);
}

int CountMinCounter::decrement(int s, int update) {
    return CountMinCounter::decrement((unsigned long) s, update);
}

int CountMinCounter::decrement(unsigned long s, int update){
    //    cout << "CountMinKeyInt: " << s << endl;
    int hashval;
    unsigned long strHash = s;
    int estimate = -1;
    for (int i = 0; i < depth; i++) {
        //hashval = (int((long) hashes[i][0] * strHash + hashes[i][1] % LONG_PRIM) % width);
        hashval = (hashes[i][0] * strHash * hashes[i][1] >> 26) % width;
//        cout << "Index: " << i << " " << hashval << endl;
        int hash = counters[i][hashval];
        hash -= update;
        counters[i][hashval] = hash;
        if (hash < estimate || estimate < 0) {
            estimate = hash;
        }
    }
//    cout << "Estimate: " << estimate << endl;
    return estimate;
}

int CountMinCounter::estimate(char *s) {
    unsigned long strHash = hashstr(s);
    return CountMinCounter::estimate(strHash);
}

int CountMinCounter::estimate(int s) {
    return CountMinCounter::estimate((unsigned long) s);
}

int CountMinCounter::estimate(unsigned long s){
    int hashval;
    unsigned long strHash = s;
    int estimate = -1;
    for(int i = 0; i < depth; i++){
        //hashval = (int((long) hashes[i][0] * strHash + hashes[i][1] % LONG_PRIM) % width);
        hashval = (hashes[i][0] * strHash * hashes[i][1] >> 26) % width;
        int hash = counters[i][hashval];
        if(hash < estimate || estimate < 0){
            estimate = hash;
        }
    }
    return estimate;
}

unsigned long CountMinCounter::hashstr(char *s) {
    hash<char*> hash_string;
    hash<unsigned long> hash_long;
//    cout << s << " => ";
    unsigned long hash = 2540;
    int c;
    c = *s++;
    int pos = 0;

    while (c) {
        int index = pos % 73;
        //cout << "Hashes:" << hash << " " << pos << " " << c << " " << hash_string_prime[index] << " " << index << endl;
        hash = hash + c * hash_string_prime[index];
        c = *s++;
        pos++;
    }
    return (hash);

}

void CountMinCounter::print() {
    cout << "Printing Data Structure" << endl;
    for (int i = 0; i < depth; i++){
        for (int j = 0; j < width; j++){
            cout << counters[i][j] << " ";
        }
        cout << endl;
    }
    cout << endl;
}

CountMinCounter::~CountMinCounter(){
    free(counters);
}
