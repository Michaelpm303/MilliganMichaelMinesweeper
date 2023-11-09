#include "Random.h"
#include <ctime>

// Redeclaring static var
mt19937 Random::random(time(0));

int Random::Int(unsigned int min, unsigned int max) {
    uniform_int_distribution<int> dist(min, max);
    return dist(random);
}

float Random::Float(float min, float max) {
    uniform_real_distribution<float> dist(min, max);
    return dist(random);
}
