#include "Tile.h"

Tile::Tile(unsigned int x, unsigned int y, unsigned int location) {
    hidden.setTexture(TextureManager::getTexture("tile_hidden"));
    hidden.setPosition(x,y);
    revealed.setTexture(TextureManager::getTexture("tile_revealed"));
    revealed.setPosition(x,y);
    flag.setTexture(TextureManager::getTexture("flag"));
    flag.setPosition(x,y);

    coordinates.x = x;
    coordinates.y = y;

    hasMine = false;
    hasAdjMine = false;
    adjMineCount = 0;
    isRevealed = false;
    isFlagged = false;
}

Sprite Tile::getHidden() {
    return hidden;
}

Sprite Tile::getRevealed() {
    return revealed;
}

Sprite Tile::getMine() {
    return mine;
}

Sprite Tile::getNumIcon() {
    return numIcon;
}

Sprite Tile::getFlag() {
    return flag;
}

bool Tile::getIsRevealed() {
    return isRevealed;
}

void Tile::setIsRevealed(bool state) {
    isRevealed = state;
}

bool Tile::getIsFlagged() {
    return isFlagged;
}

void Tile::setIsFlagged(bool state) {
    isFlagged = state;
}

bool Tile::getHasMine() {
    return hasMine;
}

void Tile::setMine() {
    mine.setTexture(TextureManager::getTexture("mine"));
    mine.setPosition(coordinates.x, coordinates.y);
    hasMine = true;
}

void Tile::setNumIcon() {
    string tempName = "number_";
    tempName += (adjMineCount + 48);
    numIcon.setTexture(TextureManager::getTexture(tempName));
    numIcon.setPosition(coordinates.x, coordinates.y);
    hasAdjMine = true;
}

bool Tile::getHasAdjMine() {
    return hasAdjMine;
}

Tile::Tile() {

}

void Tile::clearEmptyTiles() {
    for (int i = 0; i < adjTiles.size(); ++i) {
        // If the adjacent tile has NOT been revealed, has NO mine, has NO adj mines: recursively clear
        if (!adjTiles[i]->hasAdjMine && !adjTiles[i]->hasMine && !adjTiles[i]->isRevealed && !adjTiles[i]->isFlagged) {
            adjTiles[i]->isRevealed = true;
            return adjTiles[i]->clearEmptyTiles();
        }
        // If the adjacent tile has NOT been revealed, has NO mince, but HAS adj mines: normally clear
        else if (!adjTiles[i]->hasMine && !adjTiles[i]->isRevealed && !adjTiles[i]->isFlagged) {
            adjTiles[i]->isRevealed = true;
        }
    }
}






