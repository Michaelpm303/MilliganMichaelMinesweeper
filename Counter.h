#ifndef MAIN_CPP_COUNTER_H
#define MAIN_CPP_COUNTER_H
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

using namespace sf;
using namespace std;

class Counter {
    Sprite sign;
    Sprite hundredsPlace;
    Sprite tensPlace;
    Sprite onesPlace;
public:
    Counter(unsigned int x, unsigned int y);
    Vector2u coordinates;

    Sprite getSign();
    Sprite getHundreds();
    Sprite getTens();
    Sprite getOnes();

    bool negative;
    void changeHundreds(int unit);
    void changeTens(int unit);
    void changeOnes(int unit);
    void update(int number);
};


#endif
