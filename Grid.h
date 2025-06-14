#pragma once
#include <SFML/Graphics.hpp>
#include "Block.h"

class Grid {
public:
    Grid();
    void draw(sf::RenderWindow& window);
    bool checkCollision(const Block& block);
    void placeBlock(const Block& block);
    int clearLines();
    int getCell(int row, int col) const;

private:
    int grid[20][10];
    const int BLOCK_SIZE = 35;
};
