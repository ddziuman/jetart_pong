#ifndef RANDOM_H
#define RANDOM_H

#include <random>

class Random
{
private:
    // generate and seed Mersenne Twister RNG
    static std::mt19937 generate();
    static inline std::mt19937 s_mt {generate()};
    static inline std::uniform_int_distribution distrib {};
    
public:
    // random int between [min, max]
    static int get(int min, int max);
};

#endif