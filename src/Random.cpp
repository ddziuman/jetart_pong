#include "Random.h"
#include <chrono>
#include <random>
#include <algorithm>

std::mt19937 Random::generate()
{
    std::random_device rd{};

    // Create seed_seq with high-res clock and 7 random numbers from std::random_device
    std::seed_seq ss{
        static_cast<std::seed_seq::result_type>(std::chrono::steady_clock::now().time_since_epoch().count()),
            rd(), rd(), rd(), rd(), rd(), rd(), rd() };

    return std::mt19937{ ss };
}

int Random::get(int min, int max) { 
    return distrib(s_mt, std::uniform_int_distribution<int>::param_type(min, max)); 
}
