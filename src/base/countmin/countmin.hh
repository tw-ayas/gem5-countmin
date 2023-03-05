//
// Created by Ayash Twayana on 12.12.22.
//

#ifndef __MEM_CACHE_COUNTMIN_HH__
#define __MEM_CACHE_COUNTMIN_HH__

#define LONG_PRIM_1 (1 << 30)
#define LONG_PRIM_2 (1 << 29)

#include <random>
#include <set>

using namespace std;

class CountMinCounter
{
    private:
        unsigned int width;
        unsigned int depth;
        unsigned int conservative_update;
        unsigned int reset_hashes_on_reset;
        unsigned int **counters;
        char **hashes;
	    unsigned int *row_counts;
        unsigned int current_group;
        int value(int c);
        int pointValue(int c);
        std::set<std::string> countersAdded;
    public:
        CountMinCounter(int w, int d, int cons, int c_g);
        ~CountMinCounter();
        unsigned int hashstr(char *s, unsigned int row);
        unsigned int hash_function(uint16_t , unsigned int row);
        void print();
        void reset();
        void change_group_context();

        int increment(int s, int group);
        int increment(char *s, int group);
        int increment(int s, int group, int update);
        int increment(char *s, int group, int update);

        int decrement(int s);
        int decrement(char *s);
        int decrement(int s, int update);
        int decrement(char *s, int update);

        int estimate(int s, int group);
        int estimate(char *s, int group);

        int getNumberOfCounters();
        std::set<std::string> getCounterNames();
};

#endif //__MEM_CACHE_COUNTMIN_HH__


