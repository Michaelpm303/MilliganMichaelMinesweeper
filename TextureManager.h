#ifndef MAIN_CPP_TEXTUREMANAGER_H
#define MAIN_CPP_TEXTUREMANAGER_H
#include <SFML/Graphics.hpp>
#include <map>
#include <iostream>
using namespace sf;
using namespace std;

class TextureManager {
    static map<string, Texture> textures;
public:
    static void loadTexture(string textureName);
    static Texture& getTexture(string textureName);
    static void clear();
};


#endif
