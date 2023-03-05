#include "./countmin.hh"

#include <cmath>
#include <ctime>
#include <iostream>

CountMinCounter::CountMinCounter(int size, int d, int cons, int c_g)
{
    width = size / (4 * d);
    depth = d;
    conservative_update = cons;
    reset_hashes_on_reset = 0;
    std::cout << "CountMinCounter with size: " << size << "| width: " << " " << width << "| depth: " << depth << std::endl;
    auto C = new unsigned int *[depth];
    row_counts = new unsigned int[depth];
    for (int i = 0; i < depth; i++){
        C[i] = new unsigned int[width];
        for (int j = 0; j < width; j++){
            C[i][j] = 0;
        }
        row_counts[i] = 0;
    }
    counters = C;

    std::mt19937_64 gen (std::random_device{}());
    hashes = new char*[depth];
    for (int i = 0; i < depth; i++){
        hashes[i] = new char[depth];
        for(int j = 0; j < 16; j++){
            hashes[i][j] = char((gen() >> j) % width);
            cout << int(hashes[i][j]) << " ";
        }
        cout << "hashes for " << i << " " << hashes[i][0] << " " << " " << hashes[i][1] << endl;
    }

    current_group = c_g;
    cout << "current group: " <<  current_group << endl;
};

void CountMinCounter::reset(){

    std::mt19937_64 gen (std::random_device{}());
    for (int i = 0; i < depth; i++){
        for(int j = 0; j < width; j++){
            counters[i][j] = 0;
        }
        if(reset_hashes_on_reset) {
            hashes[i][0] = int(gen() * (i + 1));
            hashes[i][1] = int(gen() / (i + 1));
            cout << "hashes for " << i << " " << hashes[i][0] << " " << " " << hashes[i][1] << endl;
        }
    }
}

void CountMinCounter::change_group_context(){
    if (current_group >= 5){
        current_group = 0;
    }
    else{
        current_group++;
    }
}

int CountMinCounter::getNumberOfCounters()
{
    return countersAdded.size();
}

std::set<std::string> CountMinCounter::getCounterNames()
{
    return countersAdded;
}

int CountMinCounter::increment(int s, int group){
    return CountMinCounter::increment(s, group, 1);
}

int CountMinCounter::increment(char *s, int group){
    return  CountMinCounter::increment(s, group, 1);
}

int CountMinCounter::increment(int s, int group, int update) {
    char *str = std::to_string(s).data();
    return CountMinCounter::increment(str, group, update);
}

int CountMinCounter::increment(char *s, int group, int update)
{
    //   cout << "group: " << group << " " << s << endl;
    switch(current_group){
        case 0:
            //count all counters
            break;
        case 1:
            //count only General and some pipeline and cache
            if ((group < 1 || group > 5) && group != 11 && group != 17)
            {
                return 0;
            }
            break;
        case 2:
            //count all pipeline parameters
            if (group < 6 || group > 10)
            {
                return 0;
            }
            break;
        case 3:
            //count all l1cache parameters
            if (group < 11 || group > 15)
            {
                return 0;
            }
            break;
        case 4:
            //count all l2 cache and memory parameters
            if (group < 16 || group > 20)
            {
                return 0;
            }
            break;
        case 5:
            //cpu and pipeline parameters
            if (group < 0 || group > 10)
            {
                return 0;
            }
            break;
        case 6:
            //l1 l2 and memory parameters
            if (group < 11 || group > 20)
            {
                return 0;
            }
            break;
        case 7:
            //cpu and l1 parameters
            if (!((group >= 1 && group <= 5) || (group > 10 && group <= 15)))
            {
                return 0;
            }
            break;
        case 8:
            if (!((group >= 1 && group <= 5) || (group > 10)))
            {
                return 0;
            }
    }

    // cout << "group: " << group << endl;

    int estimate = -1;
    int hashpos[depth];
    int min = -1;

    std::string s_check(s);
    countersAdded.insert(s_check);

    for (int i = 0; i < depth; i++) {
        hashpos[i] = hashstr(s, i);
        if(counters[i][hashpos[i]] < min || min < 0){
            min = counters[i][hashpos[i]];
        }
    }
    for (int i = 0; i < depth; i++){
        if(counters[i][hashpos[i]] == min && conservative_update){
            row_counts[i] += update;
            counters[i][hashpos[i]] += update;
        }
        else if(!conservative_update){
            row_counts[i] += update;
            counters[i][hashpos[i]] += update;
        }

        if(counters[i][hashpos[i]] < estimate || estimate < 0){
            estimate =  counters[i][hashpos[i]];
        }
    }
    return estimate;
}

int CountMinCounter::decrement(int s){
    return CountMinCounter::decrement(s, 1);
}

int CountMinCounter::decrement(char *s){
    return  CountMinCounter::decrement(s, 1);
}

int CountMinCounter::decrement(int s, int update) {
    char *str = std::to_string(s).data();
    return CountMinCounter::decrement(str, update);
}

int CountMinCounter::decrement(char *s, int update){
    unsigned int hashval;
    int estimate = -1;
    for (int i = 0; i < depth; i++) {
        hashval = hashstr(s, i);
        int hash = counters[i][hashval];
        hash -= update;
        counters[i][hashval] = hash;
        if (hash < estimate || estimate < 0) {
            estimate = hash;
        }
    }
    return estimate;
}

int CountMinCounter::estimate(int s, int group) {
    char *str = std::to_string(s).data();
    return CountMinCounter::estimate(str, group);
}

int CountMinCounter::estimate(char *s, int group){

    switch(current_group){
        case 0:
            //count all counters
            break;
        case 1:
            //count only General and some pipeline and cache
            if ((group < 1 || group > 5) && group != 11 &&  group != 17)
            {
                return 0;
            }
            break;
        case 2:
            //count all pipeline parameters
            if (group < 6 || group > 10)
            {
                return 0;
            }
            break;
        case 3:
            //count all l1cache parameters
            if (group < 11 || group > 15)
            {
                return 0;
            }
            break;
        case 4:
            //count all l2 cache and memory parameters
            if (group < 16 || group > 20)
            {
                return 0;
            }
            break;
        case 5:
            //cpu and pipeline parameters
            if (group < 0 || group > 10)
            {
                return 0;
            }
            break;
        case 6:
            //l1 l2 and memory parameters
            if (group < 11 || group > 20)
            {
                return 0;
            }
            break;
        case 7:
            //cpu and l1 parameters
            if (!((group >= 1 && group <= 5) || (group > 10 && group <= 15)))
            {
                return 0;
            }
            break;
        case 8:
            if (!((group >= 1 && group <= 5) || (group > 10)))
            {
                return 0;
            }
    }

    unsigned int hashval;
    int estimate = -1;
    for(int i = 0; i < depth; i++){
        hashval = hashstr(s, i);
        int hash = counters[i][hashval];
        if(hash < estimate || estimate < 0){
            estimate = hash;
        }
    }
    return estimate;
}

int CountMinCounter::value(int c){
    if (c <= 1){
        return pointValue(c);
    }
    else{
        int v = pointValue(c + 1);
        return (1 - v)/0.00065;
    }
}

int CountMinCounter::pointValue(int c) {
    if (c == 0){
        return 0;
    }
    else{
        return pow(1.0000065, c - 1);
    }
}

unsigned int CountMinCounter::hashstr(char *s, unsigned int row){
    uint16_t hashstr = 0;
    int c = *s;
    while(c){
        hashstr = hashstr + (c >> row);
        c = *s++;
    }
    return hash_function(hashstr, row);
}

unsigned int CountMinCounter::hash_function(uint16_t key, unsigned int row) {
    int hash = 0;
    for(int i = 15; i >= 0; i--){
        uint16_t test = key & 1;
        if(test){
            if(hash){
                hash = hash ^ hashes[row][i];
            }
            else{
                hash = hashes[row][i];
            }
        }
        key = key >> 1;
    }
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
    cout << endl;
}

CountMinCounter::~CountMinCounter(){
    free(counters);
}
