#ifndef MAIN_CPP_BUTTON_H
#define MAIN_CPP_BUTTON_H
#include <SFML/Graphics.hpp>
#include "TextureManager.h"

using namespace sf;


class Button {
private:
    Sprite icon;
public:
    Vector2u coordinates;
    Sprite getIcon();
    void changeIcon(string texture);
    Button(string texture, unsigned int x, unsigned int y);
    Button();
};


#endif //MAIN_CPP_BUTTON_H
