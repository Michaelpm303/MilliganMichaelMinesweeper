#include "TextureManager.h"

map<string, Texture> TextureManager::textures;

void TextureManager::loadTexture(string textureName) {
    string path = "images/";
    path += textureName + ".png";
    textures[textureName].loadFromFile(path);
}

Texture &TextureManager::getTexture(string textureName) {
    // If texture doesn't exist
    if (textures.find(textureName) == textures.end()) {
        loadTexture(textureName);
    }

    return textures[textureName];
}

void TextureManager::clear() {
    textures.clear();
}
