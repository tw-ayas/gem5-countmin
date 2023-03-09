#include "./countmin.hh"

#include <cmath>
#include <ctime>
#include <iostream>
#include <bitset>

CountMinCounter::CountMinCounter(unsigned int size, unsigned int d, unsigned int strat, unsigned int c_g)
{
    strategy = strat;
    std::mt19937_64 gen (std::random_device{}());

    if(strategy < 2) {
        // CMS-Normal and CMS-CU
        width = (size / d) / 8;
        depth = d;

        counters = new uint64_t *[depth];
        for (int i = 0; i < depth; i++) {
            counters[i] = new uint64_t [width];
            for (int j = 0; j < width; j++) {
                counters[i][j] = 0;
            }
        }
    }
    else{
        if (strategy == 3) {
            depth = 1;
        }
        else {
            depth = d;
        }

        //CMS-Morris Counter
        width = (size / depth) / 4;
        //depth = d;
        morris_counters = new uint8_t **[depth];
        morris_constants = new uint32_t *[depth];
        for(int i = 0; i < depth; i++){
            morris_counters[i] = new uint8_t *[width];
            morris_constants[i] = new uint32_t [4];
            genMorris = new  std::mt19937_64 [4];
            for(int j = 0; j < width; j++){
                morris_counters[i][j] = new uint8_t [4];
                for(int k = 0; k < 4; k++){
                    morris_counters[i][j][k] = 0;
                    morris_constants[i][k] = gen();
                    auto v = vector<unsigned int>;
                    for( int h = 0; h < k; h++){
                        v.push(std::random{}());
                    }
                    seed_seq sd(begin(v), end(v));
                    genMorris[k] = new std::mt19937_64 (sd);
                }
            }
        }

        morris_estimate_constant = 2;   
        morris_delta_constants = new double [256];
        for (int i = 0; i < 256; i++){
            morris_delta_constants[i] = 1/(2*pow(morris_estimate_constant, i));
        }

        //if (strategy == 3){
        //    morris_counting_index = new HashMap<String, Integer>();
        //}

        counters = nullptr;
    }

    std::cout << "CountMinCounter with size: " << size << "| width: " << " " << width << "| depth: " << depth << "| with strategy: " << strategy << std::endl;

    hashes = new uint8_t *[depth];
    for (int i = 0; i < depth; i++){
        hashes[i] = new uint8_t [depth];
        for(int j = 0; j < 16; j++){
            hashes[i][j] = uint8_t((gen() >> j) % width);
            std::cout << std::bitset<6>(hashes[i][j]) << " ";
        }
        std::cout << std::endl;
    }

    current_group = c_g;
    // cout << "current group: " <<  current_group << endl;

    numCycles = 0;

    std::srand(time(nullptr));
};

int CountMinCounter::getNumberOfCounters()
{
    return countersAdded.size();
}

std::set<std::string> CountMinCounter::getCounterNames()
{
    return countersAdded;
}

uint64_t CountMinCounter::increment(int s, int group, int pc){
    return CountMinCounter::increment(s, group, pc, 1);
}

uint64_t CountMinCounter::increment(char *s, int group, int pc){
    return  CountMinCounter::increment(s, group, pc, 1);
}

uint64_t CountMinCounter::increment(int s, int group, int pc, int update) {
    char *str = std::to_string(s).data();
    return CountMinCounter::increment(str, group, pc, update);
}

uint64_t CountMinCounter::increment(char *s, int group, int pc, int update)
{
    if(!pc){
        pc = numCycles;
    }
    else{
        numCycles = pc;
    }

    std::string s_check(s);

    if(strategy == 3 && morris_counting_index.size() == width){
        return 0;
    }
    switch(current_group){
        case 0:
            //count all counters
            break;
        case 1:
            //count only General and some pipeline and cache
            if (group > 1)
            {
                return 0;
            }
            break;
        case 2:
            //count all pipeline parameters
            if (group > 2)
            {
                return 0;
            }
            break;
        case 3:
            //count all l1cache parameters
            if (group > 3)
            {
                return 0;
            }
            break;
        case 4:
            //count all l2 cache and memory parameters
            if (group > 4)
            {
                return 0;
            }
            break;
        case 5:
            //cpu and pipeline parameters
            break;
    }

    int estimate = -1;
    int actual_count = 0;
    int hashpos[depth];
    int min = -1;

    countersAdded.insert(s_check);
    
    if (strategy == 3){
         if (morris_counting_index.size() == width){
             return 0;
         }
         morris_counting_index[s_check] = morris_counting_index.size();
    }



    for (int i = 0; i < depth; i++) {
        hashpos[i] = hashstr(s, i);
        if (strategy == 1) {
            //min only kept for conservative update
            if (counters[i][hashpos[i]] < min || min < 0) {
                min = counters[i][hashpos[i]];
            }
        }
    }


    for (int i = 0; i < depth; i++){
        if(strategy == 0){
            actual_count = counters[i][hashpos[i]];
            actual_count += update;
            counters[i][hashpos[i]] = actual_count;

        }
        else if(strategy == 1){
            if(counters[i][hashpos[i]] == min) {
                actual_count = counters[i][hashpos[i]];
                actual_count += update;
                counters[i][hashpos[i]] = actual_count;
            }
            else{
                actual_count = counters[i][hashpos[i]];
            }
        }
        else if(strategy == 2 || strategy == 3){
            int column = hashpos[i];
            actual_count = increment_morris(i, column, pc, update);
        }

        if(actual_count < estimate || estimate < 0){
            estimate = actual_count;
        }
    }

    return estimate;
}

uint64_t CountMinCounter::increment_morris(int row, int column, int pc, int update)
{
    uint64_t estimate = 0;

    for(int i = 0; i < 4; i++){
        uint8_t count = morris_counters[row][column][i];
        double a = random_gen(i);

//        if (std::fmod(pc, int(pow(a, count))) == std::fmod(morris_constants[row][i], int(pow(a, count))))
        if (morris_delta_constants[count] > a)
        {
            count += update;
            morris_counters[row][column][i] = count;
        }
        estimate += uint64_t(pow(morris_estimate_constant, count));
    }

    return estimate / 4;
}

double CountMinCounter::random_gen(int i){
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return dis(genMorris[i]());
}

uint64_t CountMinCounter::decrement(int s, int group, int pc){
    return CountMinCounter::decrement(s, group, pc, 1);
}

uint64_t CountMinCounter::decrement(char *s, int group, int pc){
    return  CountMinCounter::decrement(s, group, pc, 1);
}

uint64_t CountMinCounter::decrement(int s, int group, int pc, int update) {
    char *str = std::to_string(s).data();
    return CountMinCounter::decrement(str, group, pc, update);
}

uint64_t CountMinCounter::decrement(char *s, int group, int pc, int update){
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

uint64_t CountMinCounter::estimate(int s, int group) {
    char *str = std::to_string(s).data();
    return CountMinCounter::estimate(str, group);
}

uint64_t CountMinCounter::estimate(char *s, int group){
    
    if(strategy == 3 && morris_counting_index.size()  == width){
        return 0;
    }

    switch(current_group){
        case 0:
            //count all counters
            break;
        case 1:
            //count only General and some pipeline and cache
            if (group > 1)
            {
                return 0;
            }
            break;
        case 2:
            //count all pipeline parameters
            if (group > 2)
            {
                return 0;
            }
            break;
        case 3:
            //count all l1cache parameters
            if (group > 3)
            {
                return 0;
            }
            break;
        case 4:
            //count all l2 cache and memory parameters
            if (group > 4)
                {
                    return 0;
                }
            break;
        case 5:
            //cpu and pipeline parameters
            if (group > 5)
            {
                return 0;
            }
            break;
    }

    unsigned int hashval;
    int estimate = -1;
    uint64_t hash;
    for(int i = 0; i < depth; i++){
        hashval = hashstr(s, i);
        if(strategy < 2){
            hash = counters[i][hashval];
        }
        else{
            hash = estimate_morris(i, hashval);
        }
        //std::cout << hash << endl;

        if(hash < estimate || estimate < 0){
            estimate = hash;
        }

    }
    return estimate;
}

uint64_t CountMinCounter::estimate_morris(int row, int column) {
    uint64_t estimate = 0;
    for(int i = 0; i < 4; i++) {
        double a = random_gen();
        uint8_t count = morris_counters[row][column][i];
        estimate += int(pow(morris_estimate_constant, count));
    }
    return estimate / 4;
}

unsigned int CountMinCounter::hashstr(char *s, unsigned int row){
    if(strategy == 3){
        std::string s_check(s);
        return morris_counting_index[s_check];
    }
    uint16_t hashstr = 0;
    int c = *s;
    while(c){
        hashstr = hashstr + (c);
        c = *s++;
    }
    return hash_function(hashstr, row);
}

unsigned int CountMinCounter::hash_function(uint16_t key, unsigned int row) {
    unsigned int hash = 0;

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
            if(strategy != 2) {
                cout << counters[i][j] << " ";
            }
            else{
                for(int k = 0; k < 4; k++){
                    uint32_t count = morris_counters[i][j][k];
                    cout << int(count) << " ";
                }
                cout << "| ";
            }
        }
        cout << endl;
    }
    cout << endl;
}

CountMinCounter::~CountMinCounter(){
    free(counters);
}
