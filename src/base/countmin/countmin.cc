#include "./countmin.hh"

#include <cmath>
#include <ctime>
#include <iostream>
#include <bitset>

CountMinCounter::CountMinCounter(unsigned int size, unsigned int d, unsigned int strat, unsigned int c_g) :
    genMorris (std::random_device{}())
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
        //In case of morris counting, only a single depth is created to simulate w*d counters
        depth = strategy == 3 ? 1 : d;
        width = (size / depth) / 4;
        if (strategy == 2) {
            //CMS-Morris Counter
            morris_counters = new uint8_t **[depth];
            //morris_constants = new uint32_t *[depth];
        }

        for(int i = 0; i < depth; i++){
            if (strategy == 2)
                morris_counters[i] = new uint8_t *[width];
            //morris_constants[i] = new uint32_t [4];
            for(int j = 0; j < width; j++){
                if (strategy == 2)
                    morris_counters[i][j] = new uint8_t [4];
                else
                    morris.push_back(new uint8_t [4]);
                for(int k = 0; k < 4; k++){
                    if (strategy == 2)
                        morris_counters[i][j][k] = 0;
                    else
                        morris[j][k] = 0;
                    //morris_constants[i][k] = gen();
                }
            }
        }

        morris_estimate_constant = 2;   
        morris_delta_constants = new double [256];
        for (int i = 0; i < 256; i++){
            morris_delta_constants[i] = 1/(2*pow(morris_estimate_constant, i));
        }

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

uint64_t CountMinCounter::increment(char *s, int group, int pc, int update) {
    if (!pc)
        pc = numCycles;
    else
        numCycles = pc;

    if (check_group(group) == 0)
        return 0;

    std::string s_check(s);
    //check if morris_counters are full and if not counting
    //if (strategy == 3 && morris_counting_index.size() == width && morris_counting_index.count(s_check) == 0)
    //    return 0;

    int estimate = -1;
    int actual_count = 0;
    int hashpos[depth];
    int min = -1;

    //countersAdded helps to check which counters were counted, is printed at the end of the simulation.
    countersAdded.insert(s_check);

    //check if counting is already initiated for
    if (strategy == 3 && morris_counting_index.count(s_check) == 0) {
        morris_counting_index[s_check] = morris_counting_index.size();
        morris.push_back(new uint8_t [4]);
    }

//    std::cout << s_check << ": ";
    for (int i = 0; i < depth; i++) {
        if (strategy == 3)
            hashpos[i] = morris_counting_index[s_check];
        else
            hashpos[i] = hashstr(s, i);

        //min only kept for conservative update
        if (strategy == 1 && (counters[i][hashpos[i]] < min || min < 0)) {
            min = counters[i][hashpos[i]];
        }
    }

    for (int i = 0; i < depth; i++) {
        if (strategy == 0) {
            actual_count = counters[i][hashpos[i]];
            actual_count += update;
            counters[i][hashpos[i]] = actual_count;
        } else if (strategy == 1) {
            if (counters[i][hashpos[i]] == min) {
                actual_count = counters[i][hashpos[i]];
                actual_count += update;
                counters[i][hashpos[i]] = actual_count;
            } else {
                actual_count = counters[i][hashpos[i]];
            }
        } else if (strategy == 2 || strategy == 3) {
            int column = hashpos[i];
            actual_count = increment_morris(i, column, pc, update);
        }

        if (actual_count < estimate || estimate < 0)
            estimate = actual_count;
    }
//    std::cout << estimate << std::endl;
//    print();
    return estimate;
}

uint64_t CountMinCounter::increment_morris(int row, int column, int pc, int update)
{
    uint64_t estimate = 0;
    while(update > 0) {
        estimate = 0;
        for(int i = 0; i < 4; i++){
            uint8_t count;
            if (strategy == 2)
                count = morris_counters[row][column][i];
            else
                count = morris[column][i];
            double a = random_gen();
            if (morris_delta_constants[count] > a)
            {
                count++;
                if (strategy == 2)
                    morris_counters[row][column][i] = count;
                else
                    morris[column][i] = count;
            }
            estimate += uint64_t(pow(morris_estimate_constant, count));
        }
        update--;
    }

    return estimate / 4;
}

double CountMinCounter::random_gen(){
    std::uniform_real_distribution<double> dis(0.0, 1.0);
    return dis(genMorris);
}

uint64_t CountMinCounter::estimate(int s, int group) {
    char *str = std::to_string(s).data();
    return CountMinCounter::estimate(str, group);
}

uint64_t CountMinCounter::estimate(char *s, int group){

    std::string s_check(s);

    if(check_group(group) == 0)
        return 0;

    //if(strategy == 3 && morris_counting_index.size() == width && morris_counting_index.count(s_check) == 0)
    //    return 0;

    unsigned int hashval;
    int estimate = -1;
    uint64_t hash;
    for(int i = 0; i < depth; i++){
        if(strategy == 3)
            hashval = morris_counting_index[s_check];
        else
            hashval = hashstr(s, i);
        if(strategy < 2)
            hash = counters[i][hashval];
        else
            hash = estimate_morris(i, hashval);

        if(hash < estimate || estimate < 0)
            estimate = hash;

    }
    return estimate;
}

uint64_t CountMinCounter::estimate_morris(int row, int column) {
    uint32_t estimate = 0;
    for(int i = 0; i < 4; i++) {
        double a = random_gen();
        uint8_t count;
        if (strategy == 2)
            count = morris_counters[row][column][i];
        else
            count = morris[column][i];
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

unsigned int CountMinCounter::check_group(int group){
    switch(current_group){
        case 0:
            //count all counters
            break;
        case 1:
            if (group > 1)
                return 0;
            break;
        case 2:
            if (group > 2)
                return 0;
            break;
        case 3:
            if (group > 3)
                return 0;
            break;
        case 4:
            if (group > 4)
                return 0;
            break;
    }
    return 1;
}

void CountMinCounter::print() {
    cout << "Printing Data Structure" << endl;
    if (strategy != 3) {
    for (int i = 0; i < depth; i++){
        for (int j = 0; j < width; j++){
            if(strategy < 2) {
                cout << counters[i][j] << " ";
            }
            else{
                for(int k = 0; k < 4; k++){
                    uint32_t count;
                    if (strategy == 2)
                        count = morris_counters[i][j][k];
                    else
                        count = morris[j][k];
                    cout << int(count) << " ";
                }
                cout << "| ";
            }
        }
        cout << endl;
    }
    }
    else{
        for (auto *counter : morris){
            for(int k = 0; k < 4; k++){
                uint32_t count = counter[k];
                cout << int(count) << " ";
            }
            cout << "| ";
        }
    }
    cout << endl;
}

CountMinCounter::~CountMinCounter(){
    free(counters);
}
