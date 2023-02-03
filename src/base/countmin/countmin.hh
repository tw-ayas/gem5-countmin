//
// Created by Ayash Twayana on 12.12.22.
//

#ifndef __MEM_CACHE_COUNTMIN_HH__
#define __MEM_CACHE_COUNTMIN_HH__

#define LONG_PRIM (1 << 30)

#include <random>

using namespace std;

class CountMinCounter
{
    private:
        unsigned int width;
        unsigned int depth;
        unsigned int **counters;
        unsigned int **hashes;
	unsigned int hash_string_prime[73];
        unsigned int current_group;

    public:
        CountMinCounter(int w, int d);
        ~CountMinCounter();
        unsigned long hashstr(char *s);
        void print();
        void reset();
        void change_group_context();
        int increment(int s);
        int increment(char *s);
        int increment(int s, int update);
        int increment(unsigned long s, int update);
        int increment(char *s, int update);
        int decrement(int s);
        int decrement(char *s);
        int decrement(int s, int update);
        int decrement(unsigned long s, int update);
        int decrement(char *s, int update);

        int estimate(int s);
        int estimate(char *s);
        int estimate(unsigned long s);
};

#endif //__MEM_CACHE_COUNTMIN_HH__

