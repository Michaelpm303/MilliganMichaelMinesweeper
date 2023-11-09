#include "Counter.h"

Counter::Counter(unsigned int x, unsigned int y) {
    sign.setTexture(TextureManager::getTexture("digits"));
    sign.setTextureRect(IntRect((21 * 10),0, 21,32));
    sign.setPosition(x, y);

    hundredsPlace.setTexture(TextureManager::getTexture("digits"));
    hundredsPlace.setTextureRect(IntRect(0,0, 21,32));
    hundredsPlace.setPosition(x + 21, y);

    tensPlace.setTexture(TextureManager::getTexture("digits"));
    tensPlace.setTextureRect(IntRect(0,0, 21,32));
    tensPlace.setPosition(x + (21 * 2), y);

    onesPlace.setTexture(TextureManager::getTexture("digits"));
    onesPlace.setTextureRect(IntRect(0,0, 21,32));
    onesPlace.setPosition(x + (21 * 3), y);

    coordinates.x = x;
    coordinates.y = y;

    negative = false;
}

void Counter::changeHundreds(int unit) {
    hundredsPlace.setTextureRect(IntRect((21 * unit),0, (21),32));
}

void Counter::changeTens(int unit) {
    tensPlace.setTextureRect(IntRect((21 * unit),0, (21),32));
}

void Counter::changeOnes(int unit) {
    onesPlace.setTextureRect(IntRect((21 * unit),0, (21),32));
}

void Counter::update(int number) {
    // If negative
    if (number < 0) {
        negative = true;
        number = 0 - number;
    }
    // If positive
    else {
        negative = false;
    }

    if (number >= 100) {
        changeHundreds(number/100);
        changeTens(number%100/10);
        changeOnes(number%100%10);
    }
    if (number >= 10 && number < 100) {
        changeHundreds(0);
        changeTens(number/10);
        changeOnes(number%10);
    }
    if (number >=0 && number < 10) {
        changeHundreds(0);
        changeTens(0);
        changeOnes(number);
    }

}

Sprite Counter::getSign() {
    return sign;
}

Sprite Counter::getHundreds() {
    return hundredsPlace;
}

Sprite Counter::getTens() {
    return tensPlace;
}

Sprite Counter::getOnes() {
    return onesPlace;
}
