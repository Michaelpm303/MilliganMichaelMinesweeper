#include <SFML/Graphics.hpp>
#include <iostream>
#include <fstream>
#include "Board.h"
#include "Random.h"
#include "Tile.h"
#include "Button.h"
#include "TextureManager.h"

using namespace sf;
using namespace std;

Board configBoard();

int main()
{
    // Configuring board and grid of tiles
    Board board = configBoard();
    RenderWindow window(VideoMode(board.getColumns() * 32, board.getRows() * 32 + 100), "Minesweeper");

    //board.initializeTextures();
    board.randomizeGrid();

    while (window.isOpen())
    {
        window.clear();
        board.checkForWin();
        // Drawing the tiles
        for (int i = 0; i < board.tileGrid.size(); ++i) {
            for (int j = 0; j < board.tileGrid.at(i).size(); ++j) {
                // Then check if it is still hidden
                if (!board.tileGrid[i][j].getIsRevealed()) {
                    window.draw(board.tileGrid[i][j].getHidden());
                    // If debug is on and the tile has a mine
                    if (board.debugToggled && board.tileGrid[i][j].getHasMine()) {
                        window.draw(board.tileGrid[i][j].getMine());
                    }
                    // Also if flagged, both can occur at the same time
                    if (board.tileGrid[i][j].getIsFlagged()) {
                        window.draw(board.tileGrid[i][j].getFlag());
                    }
                }
                // The rest are for revealed tiles not flagged
                else {
                    window.draw(board.tileGrid[i][j].getRevealed());

                    // If the tile HAS a mine
                    if (board.tileGrid[i][j].getHasMine()){
                        // Special case: if game is over and all mines are revealed, but the mine has a flag
                        if (board.tileGrid[i][j].getIsFlagged()) {
                            window.draw(board.tileGrid[i][j].getFlag());
                        }
                        window.draw(board.tileGrid[i][j].getMine());
                    }
                    // If the tile has NO mine & HAS adj mine
                    else if (board.tileGrid[i][j].getHasAdjMine()) {
                        window.draw(board.tileGrid[i][j].getNumIcon());
                    }

                    // If the tile has NO mine & has NO adj mine
                    else {
                       board.tileGrid[i][j].clearEmptyTiles();
                    }

                }
            }
        }
        // Drawing the buttons
        window.draw(board.smileyButton.getIcon());
        window.draw(board.debug.getIcon());
        window.draw(board.test1.getIcon());
        window.draw(board.test2.getIcon());
        window.draw(board.test3.getIcon());
        // Drawing Counter
        if (board.counter.negative) {
            window.draw(board.counter.getSign());
        }
        window.draw(board.counter.getHundreds());
        window.draw(board.counter.getTens());
        window.draw(board.counter.getOnes());

        // Event control
        sf::Event event;
        while (window.pollEvent(event))
        {
            switch (event.type)
            {
                case Event::Closed: {
                    window.close();
                    break;
                }
                case Event::MouseButtonPressed: {

                    // If Right-clicked within grid boundaries
                    if (event.mouseButton.button == Mouse::Right) {
                        if (event.mouseButton.x >= 0 && event.mouseButton.x <= (board.getColumns() * 32)) {
                            if (event.mouseButton.y >= 0 && event.mouseButton.y <= (board.getRows() * 32) && !board.gameOver) {
                                for (int i = 0; i < board.getRows(); ++i) {
                                    for (int j = 0; j < board.getColumns(); ++j) {
                                        if (event.mouseButton.x >= board.tileGrid[i][j].coordinates.x && event.mouseButton.x < (board.tileGrid[i][j].coordinates.x + 32)) {
                                            if (event.mouseButton.y >= board.tileGrid[i][j].coordinates.y && event.mouseButton.y < (board.tileGrid[i][j].coordinates.y + 32)) {
                                                // Flags only work on hidden tiles
                                                if (!board.tileGrid[i][j].getIsRevealed()) {
                                                    // Getting rid of flag and adding displayed mine count back
                                                    if (board.tileGrid[i][j].getIsFlagged()) {
                                                        board.tileGrid[i][j].setIsFlagged(false);
                                                        board.addPseudoMine();
                                                        //Updating counter based on the apparent mine count
                                                        board.counter.update(board.getPseudoMineCount());
                                                    }
                                                        // Adding a flag and subtracting from displayed mine count
                                                    else {
                                                        board.tileGrid[i][j].setIsFlagged(true);
                                                        board.subPseudoMine();
                                                        //Updating counter based on the apparent mine count
                                                        board.counter.update(board.getPseudoMineCount());
                                                    }
                                                }
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }

                    else if (event.mouseButton.button == Mouse::Left) {

                        // If Left-clicked within window boundaries
                        if (0 <= event.mouseButton.x && event.mouseButton.x <= (board.getColumns() * 32)) {
                            if (0 <= event.mouseButton.y && event.mouseButton.y <= (board.getRows() * 32 + 100)) {
                                // If clicked on tile grid AND game is not over
                                if (event.mouseButton.y <= board.getRows() * 32 && !board.gameOver) {
                                    for (int i = 0; i < board.getRows(); ++i) {
                                        bool tileFound = false;
                                        for (int j = 0; j < board.getColumns(); ++j) {
                                            if (event.mouseButton.x >= board.tileGrid[i][j].coordinates.x && event.mouseButton.x < (board.tileGrid[i][j].coordinates.x + 32)) {
                                                if (event.mouseButton.y >= board.tileGrid[i][j].coordinates.y && event.mouseButton.y < (board.tileGrid[i][j].coordinates.y + 32)) {
                                                    tileFound = true;
                                                    // If has no flag:
                                                    if (!board.tileGrid[i][j].getIsFlagged()) {
                                                        // If has no mine
                                                        if (!board.tileGrid[i][j].getHasMine()) {
                                                            board.tileGrid[i][j].setIsRevealed(true);
                                                            //board.tileGrid[i][j].setIsRevealed(true);
                                                        }
                                                        // If clicked tile has a mine
                                                        else {
                                                            board.loseGame();
                                                        }
                                                    }
                                                    break;
                                                }
                                                if (tileFound) {
                                                    break;
                                                }
                                            }
                                        }
                                    }
                                }
                                // If clicked on other buttons
                                else {
                                    // Correct y bounds of buttons
                                    if (event.mouseButton.y <= (board.getRows() * 32 + 64) && event.mouseButton.y >= board.getRows() * 32) {
                                        // Smiley button 'x' bounds
                                        if (event.mouseButton.x >= (board.getColumns() * 32) / 2 - 32 && event.mouseButton.x < (board.getColumns() * 32) / 2 + 32) {
                                            board.randomizeGrid();
                                        }
                                        // Test 3 button 'x' bounds
                                        else if (event.mouseButton.x >= ((board.getColumns() * 32) / 2 + 32) + (64 * 4) && event.mouseButton.x < ((board.getColumns() * 32) / 2 + 32) + (64 * 5)) {
                                            board.loadGrid("testboard3");
                                        }
                                        // Test 2 button 'x' bounds
                                        else if (event.mouseButton.x >= ((board.getColumns() * 32) / 2 + 32) + (64 * 3) && event.mouseButton.x < ((board.getColumns() * 32) / 2 + 32) + (64 * 4)) {
                                            board.loadGrid("testboard2");
                                        }
                                        // Test 1 button 'x' bounds
                                        else if (event.mouseButton.x >= ((board.getColumns() * 32) / 2 + 32) + (64 * 2) && event.mouseButton.x < ((board.getColumns() * 32) / 2 + 32) + (64 * 3)) {
                                            board.loadGrid("testboard1");
                                        }
                                        // Debug button 'x' bounds
                                        else if (event.mouseButton.x >= ((board.getColumns() * 32) / 2 + 32) + (64) && event.mouseButton.x < ((board.getColumns() * 32) / 2 + 32) + (64 * 2)) {
                                            // Turning debug on
                                            if (!board.debugToggled) {
                                                board.debugToggled = true;
                                            }
                                            // Turning debug off
                                            else {
                                                board.debugToggled = false;

                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        window.display();
    }

return 0;
}

Board configBoard() {
    ifstream input("boards/config.cfg");

    unsigned int columns;
    input >> columns;
    unsigned int rows;
    input >> rows;
    int mineCount;
    input >> mineCount;
    Board board(columns, rows, mineCount);

    return board;
}

//void loadTextures() {
//    TextureManager::loadTexture("mine");
//    TextureManager::loadTexture("tile_hidden");
//    TextureManager::loadTexture("tile_revealed");
//    TextureManager::loadTexture("number_1");
//    TextureManager::loadTexture("number_2");
//    TextureManager::loadTexture("number_3");
//    TextureManager::loadTexture("number_4");
//    TextureManager::loadTexture("number_5");
//    TextureManager::loadTexture("number_6");
//    TextureManager::loadTexture("number_7");
//    TextureManager::loadTexture("number_8");
//    TextureManager::loadTexture("flag");
//    TextureManager::loadTexture("face_happy");
//    TextureManager::loadTexture("face_win");
//    TextureManager::loadTexture("face_lose");
//    TextureManager::loadTexture("digits");
//    TextureManager::loadTexture("debug");
//    TextureManager::loadTexture("test_1");
//    TextureManager::loadTexture("test_2");
//    TextureManager::loadTexture("test_3");
//}