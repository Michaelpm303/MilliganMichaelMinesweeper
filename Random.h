#ifndef MAIN_CPP_RANDOM_H
#define MAIN_CPP_RANDOM_H
#include <random>
using namespace std;

class Random {
    static mt19937 random;
public:
    static int Int(unsigned int min, unsigned int max);
    static float Float(float min, float max);
};


#endif
