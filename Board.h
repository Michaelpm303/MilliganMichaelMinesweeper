#ifndef MAIN_CPP_BOARD_H
#define MAIN_CPP_BOARD_H
#include <SFML/Window.hpp>
#include <SFML/Graphics.hpp>
#include <vector>
#include <map>
#include "Tile.h"
#include "Random.h"
#include "Button.h"
#include "Counter.h"

using namespace std;
using namespace sf;


class Board {
private:
    unsigned int columns;
    unsigned int rows;
    unsigned int configuredMineCount;
    unsigned int mineCount;
    int pseudoMineCount;
    unsigned int tileCount;
    unsigned int revealedCount;
    map<unsigned int, bool> mineLocations;

public:
    Board(unsigned int columns, unsigned int rows, int mineCount);

    bool gameOver;
    void loseGame();
    void winGame();
    void checkForWin();

    Button smileyButton;
    Button debug;
    bool debugToggled;
    Button test1;
    Button test2;
    Button test3;
    Counter counter;

    Vector2u coordinates;
    vector<vector<Tile>> tileGrid;

    void setColumns(unsigned int columns);
    void setRows(unsigned int rows);
    void setMineCount(unsigned int mineCount);
    void revealTile(Tile& tile);
    void addRevealedTiles(unsigned int num);
    void addPseudoMine();
    void subPseudoMine();

    unsigned int getColumns();
    unsigned int getRows();
    unsigned int getMineCount();
    unsigned int getTileCount();
    unsigned int getRevealedCount();
    int getPseudoMineCount();
    map<unsigned int, bool> getMineLocations();

    void randomizeGrid();
    void loadGrid(string boardName);
    void setUpNeighbors();

};


#endif //MAIN_CPP_BOARD_H
