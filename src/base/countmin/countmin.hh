//
// Created by Ayash Twayana on 12.12.22.
//

#ifndef __MEM_CACHE_COUNTMIN_HH__
#define __MEM_CACHE_COUNTMIN_HH__

#define LONG_PRIM (1 << 30) - 1

using namespace std;

class CountMinCounter
{
    private:
        unsigned int width;
        unsigned int depth;
        unsigned int **counters;
        unsigned int **hashes;

    public:
        CountMinCounter(float w, float d);
        static int hashstr(char *s);
        void print();
        void increment(int s);
        void increment(char *s);
        void increment(int s, int update);
        void increment(char *s, int update);
        int estimate(int s);
        int estimate(char *s);
};

#endif //__MEM_CACHE_COUNTMIN_HH__

