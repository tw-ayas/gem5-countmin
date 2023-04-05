//
// Created by Ayash Twayana on 12.12.22.
//

#ifndef __MEM_CACHE_COUNTMIN_HH__
#define __MEM_CACHE_COUNTMIN_HH__

#define LONG_PRIM_1 (1 << 30)
#define LONG_PRIM_2 (1 << 29)

#include <random>
#include <set>
#include <map>

using namespace std;

class CountMinCounter
{
private:
    unsigned int width;
    unsigned int depth;
    //stragtegy will be normal, conservative update and morris counters group
    unsigned int strategy;

    //We use numCycles for MorrisCounter saved everytime when cpu sends numCycles
    uint64_t numCycles;

    //2 dimensional matrix with normal counters
    uint64_t **counters;

    //2 dimensional matrix with a group of 4 morris counters such that the width can be increased by twice
    //we have 4 morris constants for each row of CMS
    uint8_t ***morris_counters;
    uint32_t **morris_constants; // a
    std::vector<uint8_t[4]> morris;
    double *morris_delta_constants;
    float morris_estimate_constant;
    std::mt19937_64 genMorris;
    std::map<std::string, int> morris_counting_index;

    //hashes is a 2 dimensional Matrix depth X 16 8 byte integers (char)
    uint8_t **hashes;

    uint64_t *row_counts;
    uint8_t current_group;
    int value(int c);
    int pointValue(int c);
    std::set<std::string> countersAdded;

    double random_gen();

public:
    CountMinCounter(unsigned int w, unsigned int d, unsigned int strategy, unsigned int c_g);
    ~CountMinCounter();
    unsigned int hashstr(char *s, unsigned int row);
    unsigned int hash_function(uint16_t , unsigned int row);
    unsigned int check_group(int group);
    void print();
    void reset();
    void change_group_context();

    uint64_t increment(int s, int group, int pc);
    uint64_t increment(char *s, int group, int pc);
    uint64_t increment(int s, int group, int pc, int update);
    uint64_t increment(char *s, int group, int pc, int update);

    uint64_t increment_morris(int row, int column, int pc, int update);

    uint64_t estimate(int s, int group);
    uint64_t estimate(char *s, int group);
    uint64_t estimate_morris(int row, int column);

    int getNumberOfCounters();
    std::set<std::string> getCounterNames();
};

#endif //__MEM_CACHE_COUNTMIN_HH__


