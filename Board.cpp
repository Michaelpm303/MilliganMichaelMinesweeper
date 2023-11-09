#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Board.h"
#include "Button.h"
#include "Counter.h"

using namespace std;

void Board::setColumns(unsigned int columns) {
    this->columns = columns;
}

void Board::setRows(unsigned int rows) {
    this->rows = rows;
}

void Board::setMineCount(unsigned int mineCount) {
    this->mineCount = mineCount;
}

void Board::revealTile(Tile& tile) {
    tile.setIsRevealed(true);
    revealedCount++;
}

void Board::addRevealedTiles(unsigned int num) {
    revealedCount += num;
}

void Board::addPseudoMine() {
    pseudoMineCount++;

}

void Board::subPseudoMine() {
    pseudoMineCount--;
}

unsigned int Board::getColumns() {
    return columns;
}

unsigned int Board::getRows() {
    return rows;
}

unsigned int Board::getMineCount() {
    return mineCount;
}

int Board::getPseudoMineCount() {
    return pseudoMineCount;
}

unsigned int Board::getTileCount() {
    return columns * rows;
}

unsigned int Board::getRevealedCount() {
    return revealedCount;
}

map<unsigned int, bool> Board::getMineLocations() {
    return mineLocations;
}

//vector<unsigned int> Board::getMineLocations() {
//    return mineLocations;
//}

// Constructor for board with initializer list of buttons
Board::Board(unsigned int columns, unsigned int rows, int mineCount)
: smileyButton("face_happy", ((columns * 32) / 2) - 32, rows * 32),
test3("test_3", ((columns * 32) / 2) + 32 + (64 * 4), rows * 32),
test2("test_2", ((columns * 32) / 2) + 32 + (64 * 3), rows * 32),
test1("test_1", ((columns * 32) / 2) + 32 + (64 * 2), rows * 32),
debug("debug", ((columns * 32) / 2) + 32 + 64, rows * 32),
counter(0,rows * 32)
{
    this->columns = columns;
    this->rows = rows;
    configuredMineCount = mineCount;
    pseudoMineCount = mineCount;
    tileCount = columns * rows;
    debugToggled = false;
    gameOver = false;
    revealedCount = 0;
}

void Board::loseGame() {
    gameOver = true;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (tileGrid[i][j].getHasMine()) {
                tileGrid[i][j].setIsRevealed(true);
            }
        }
    }
    smileyButton.changeIcon("face_lose");
}

void Board::winGame() {
    gameOver = true;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (tileGrid[i][j].getHasMine()) {
                tileGrid[i][j].setIsFlagged(true);
            }
        }
    }
    debugToggled = false;
    smileyButton.changeIcon("face_win");
    counter.update(0);
}

void Board::checkForWin() {
    unsigned int revealedTiles = 0;
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            if (tileGrid[i][j].getIsRevealed()) {
                revealedTiles++;
            }
        }
    }
    if (revealedTiles >= (tileCount - mineCount)) {
        winGame();
    }
}

void Board::randomizeGrid() {
    // Tiles need to be loaded in a separate loop than the loop assigning adjacent tiles
    // because it changes place in memory when pushed back, changing pointer values.
    coordinates.x = 0;
    coordinates.y = 0;
    unsigned int tileLocation = 0;
    tileGrid.clear();
    mineLocations.clear();
    gameOver = false;
    smileyButton.changeIcon("face_happy");
    mineCount = configuredMineCount;
    revealedCount = 0;
    pseudoMineCount = configuredMineCount;
    counter.update(pseudoMineCount);

    // Generating Mine locations
    // Clearing for each board reset
    unsigned int generatedMines = 0;
    while (generatedMines < mineCount) {
        // Making sure mines do not overlap
        bool locationOccupied = false;
        unsigned int location = Random::Int(0,tileCount - 1);

        auto iter = mineLocations.begin();
        for (; iter != mineLocations.end(); ++iter) {
            // If the location is already occupied
            if (location == iter->first) {
                locationOccupied = true;
            }
        }
        // If the location is free
        if (!locationOccupied) {
            mineLocations.emplace(location, true);
            generatedMines++;
        }
    }

    // Creating tile objects with unique locations
    for (int i = 0; i < rows; ++i) {
        tileGrid.resize(i + 1);
        for (int j = 0; j < columns; ++j) {
            Tile tile(coordinates.x, coordinates.y, tileLocation);
            if (mineLocations[tileLocation]) {
                tile.setMine();
            }
            tileGrid[i].push_back(tile);
            tileLocation++;

            coordinates.x += 32;
        }

        coordinates.y += 32;
        coordinates.x = 0;
    }
    setUpNeighbors();
}

void Board::loadGrid(string boardName) {
    coordinates.x = 0;
    coordinates.y = 0;
    unsigned int tileLocation = 0;
    tileGrid.clear();
    mineLocations.clear();
    gameOver = false;
    smileyButton.changeIcon("face_happy");
    mineCount = 0;
    revealedCount = 0;

    string path = "boards/" + boardName + ".brd";
    ifstream input(path);

    string boardRow;
    unsigned int i = 0;
    // Iterates one row per loop
    while (getline(input, boardRow)) {
        tileGrid.resize(i + 1);
        stringstream rowStream(boardRow);
        for (int j = 0; j < columns; ++j) {
            char mineCheck;
            rowStream.get(mineCheck);
            Tile tile(coordinates.x, coordinates.y, tileLocation);
            if (mineCheck == '1') {
                tile.setMine();
                mineCount++;
            }

            tileGrid[i].push_back(tile);
            tileLocation++;
            coordinates.x += 32;
        }
        coordinates.y += 32;
        coordinates.x = 0;
        i++;
    }
    pseudoMineCount = mineCount;
    counter.update(pseudoMineCount);

    setUpNeighbors();
}

void Board::setUpNeighbors() {
    // Adding neighbors
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            // If in the center of grid (or bottom row)
            if (i != 0 && j != 0 && j != (columns - 1)) {

                // Up
                tileGrid[i][j].adjTiles.push_back(&tileGrid[i - 1][j]);
                if (tileGrid[i - 1][j].getHasMine()) {
                    tileGrid[i][j].adjMineCount++;
                }
                tileGrid[i - 1][j].adjTiles.push_back(&tileGrid[i][j]);
                if (tileGrid[i][j].getHasMine()) {
                    tileGrid[i - 1][j].adjMineCount++;
                }

                // Up-Left
                tileGrid[i][j].adjTiles.push_back(&tileGrid[i - 1][j - 1]);
                if (tileGrid[i - 1][j - 1].getHasMine()) {
                    tileGrid[i][j].adjMineCount++;
                }
                tileGrid[i - 1][j - 1].adjTiles.push_back(&tileGrid[i][j]);
                if (tileGrid[i][j].getHasMine()) {
                    tileGrid[i - 1][j - 1].adjMineCount++;
                }

                // Up-Right
                tileGrid[i][j].adjTiles.push_back(&tileGrid[i - 1][j + 1]);
                if (tileGrid[i - 1][j + 1].getHasMine()) {
                    tileGrid[i][j].adjMineCount++;
                }
                tileGrid[i - 1][j + 1].adjTiles.push_back(&tileGrid[i][j]);
                if (tileGrid[i][j].getHasMine()) {
                    tileGrid[i - 1][j + 1].adjMineCount++;
                }

                // Left
                tileGrid[i][j].adjTiles.push_back(&tileGrid[i][j - 1]);
                if (tileGrid[i][j - 1].getHasMine()) {
                    tileGrid[i][j].adjMineCount++;
                }
                tileGrid[i][j - 1].adjTiles.push_back(&tileGrid[i][j]);
                if (tileGrid[i][j].getHasMine()) {
                    tileGrid[i][j - 1].adjMineCount++;
                }
            }

            // If at the top row
            if (i == 0 && j != 0) {
                // Left
                tileGrid[i][j].adjTiles.push_back(&tileGrid[i][j - 1]);
                if (tileGrid[i][j - 1].getHasMine()) {
                    tileGrid[i][j].adjMineCount++;
                }
                tileGrid[i][j - 1].adjTiles.push_back(&tileGrid[i][j]);
                if (tileGrid[i][j].getHasMine()) {
                    tileGrid[i][j - 1].adjMineCount++;
                }
            }

            // If on the left side column
            if (i != 0 && j == 0) {
                // Up
                tileGrid[i][j].adjTiles.push_back(&tileGrid[i - 1][j]);
                if (tileGrid[i - 1][j].getHasMine()) {
                    tileGrid[i][j].adjMineCount++;
                }
                tileGrid[i - 1][j].adjTiles.push_back(&tileGrid[i][j]);
                if (tileGrid[i][j].getHasMine()) {
                    tileGrid[i - 1][j].adjMineCount++;
                }

                // Up-Right
                tileGrid[i][j].adjTiles.push_back(&tileGrid[i - 1][j + 1]);
                if (tileGrid[i - 1][j + 1].getHasMine()) {
                    tileGrid[i][j].adjMineCount++;
                }
                tileGrid[i - 1][j + 1].adjTiles.push_back(&tileGrid[i][j]);
                if (tileGrid[i][j].getHasMine()) {
                    tileGrid[i - 1][j + 1].adjMineCount++;
                }
            }

            // If on the right side column
            if (i != 0 && j == (columns - 1)) {
                // Up
                tileGrid[i][j].adjTiles.push_back(&tileGrid[i - 1][j]);
                if (tileGrid[i - 1][j].getHasMine()) {
                    tileGrid[i][j].adjMineCount++;
                }
                tileGrid[i - 1][j].adjTiles.push_back(&tileGrid[i][j]);
                if (tileGrid[i][j].getHasMine()) {
                    tileGrid[i - 1][j].adjMineCount++;
                }

                // Up-Left
                tileGrid[i][j].adjTiles.push_back(&tileGrid[i - 1][j - 1]);
                if (tileGrid[i - 1][j - 1].getHasMine()) {
                    tileGrid[i][j].adjMineCount++;
                }
                tileGrid[i - 1][j - 1].adjTiles.push_back(&tileGrid[i][j]);
                if (tileGrid[i][j].getHasMine()) {
                    tileGrid[i - 1][j - 1].adjMineCount++;
                }

                // Left
                tileGrid[i][j].adjTiles.push_back(&tileGrid[i][j - 1]);
                if (tileGrid[i][j - 1].getHasMine()) {
                    tileGrid[i][j].adjMineCount++;
                }
                tileGrid[i][j - 1].adjTiles.push_back(&tileGrid[i][j]);
                if (tileGrid[i][j].getHasMine()) {
                    tileGrid[i][j - 1].adjMineCount++;
                }
            }
        }
    }
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < columns; ++j) {
            // If the tile has adjacent mines
            if (tileGrid[i][j].adjMineCount != 0) {
                tileGrid[i][j].setNumIcon();
            }
        }
    }
}





