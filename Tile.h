#ifndef MAIN_CPP_TILE_H
#define MAIN_CPP_TILE_H
#include <SFML/Graphics.hpp>
#include <map>
#include "TextureManager.h"

using namespace sf;
using namespace std;

class Tile {
private:
    Sprite hidden;
    Sprite revealed;
    Sprite mine;
    Sprite numIcon;
    Sprite flag;
    bool isRevealed;
    bool isFlagged;
    bool hasMine;
    bool hasAdjMine;
public:
    vector<Tile*> adjTiles;
    unsigned int adjMineCount;
    Vector2u coordinates;

    Tile();
    Tile(unsigned int x, unsigned int y, unsigned int location);

    Sprite getHidden();
    Sprite getRevealed();
    Sprite getMine();
    Sprite getNumIcon();
    Sprite getFlag();

    bool getIsRevealed();
    void setIsRevealed(bool state);
    bool getIsFlagged();
    void setIsFlagged(bool state);
    bool getHasMine();
    void setMine();
    void setNumIcon();
    bool getHasAdjMine();

    void clearEmptyTiles();
};


#endif //MAIN_CPP_TILE_H
