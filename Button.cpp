#include "Button.h"

Button::Button(string texture, unsigned int x, unsigned int y) {
    icon.setTexture(TextureManager::getTexture(texture));
    icon.setPosition(x,y);

    coordinates.x = x;
    coordinates.y = y;

}

Button::Button() {

}

Sprite Button::getIcon() {
    return icon;
}

void Button::changeIcon(string texture) {
    icon.setTexture(TextureManager::getTexture(texture));
}
