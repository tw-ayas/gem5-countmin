//
// Created by Ayash Twayana on 12.12.22.
//

#ifndef __MEM_CACHE_COUNTMIN_HH__
#define __MEM_CACHE_COUNTMIN_HH__

#include <random>

#define LONG_PRIM (1 << 30) - 1

static const unsigned int prime_array[] = {503, 509, 521, 523, 541, 547, 557, 563, 569, 571, 577, 587, 593, 599,
                     601, 607, 613, 617, 619, 631, 641, 643, 647, 653, 659, 661, 673, 677, 683, 691,
                     701, 709, 719, 727, 733, 739, 743, 751, 757, 761, 769, 773, 787, 797,
                     809, 811, 821, 823, 827, 829, 839, 853, 857, 859, 863, 877, 881, 883, 887,
                     907, 911, 919, 929, 937, 941, 947, 953, 967, 971, 977, 983, 991, 997
};

using namespace std;

class CountMinCounter
{
    private:
        unsigned int width;
        unsigned int depth;
        unsigned int **counters;
        unsigned int **hashes;
        hash<long> hash_row;
	unsigned int *hash_string_prime;

    public:
        CountMinCounter(float w, float d);
        ~CountMinCounter();
        unsigned long hashstr(char *s);
        void print();
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

