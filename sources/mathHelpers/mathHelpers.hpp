#ifndef __MATH_CONSTANTS__
#define __MATH_CONSTANTS__

#include <chrono>
#include <random>

#define PI 3.14159265358979311600
#define TO_RAD(X) (PI/180 * X)
#define MAX_ID 2147483630

// Got this from my old competitive programming template
inline long long mh_random(long long __mn__, long long __mx__) 
{ 
    std::mt19937 rng(std::chrono::steady_clock::now().time_since_epoch().count()); 
    return std::uniform_int_distribution<long long>(__mn__, __mx__)(rng);
}

#endif