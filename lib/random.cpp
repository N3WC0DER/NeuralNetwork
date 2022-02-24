#include <iostream>
#include "random.h"

using namespace std;

int getRandomNumber(const int min, const int max) {
    static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
    return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}