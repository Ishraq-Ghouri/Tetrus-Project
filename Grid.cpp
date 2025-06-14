#include "Grid.h"
#include "Block.h"
#include "colors.h" // Centralized color definitions

Grid::Grid() {
    for (int y = 0; y < 20; ++y) {
        for (int x = 0; x < 10; ++x) {
            grid[y][x] = 0;
        }
    }
}

void Grid::draw(sf::RenderWindow& window) {
    sf::RectangleShape rect(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));

    for (int y = 0; y < 20; ++y) {
        for (int x = 0; x < 10; ++x) {
            int colorID = grid[y][x];
            if (colorID) {
                rect.setPosition(x * BLOCK_SIZE, y * BLOCK_SIZE);
                rect.setFillColor(COLORS[colorID]);
                window.draw(rect);
            }
        }
    }
}

bool Grid::checkCollision(const Block& block) {
    for (int i = 0; i < 4; ++i) {
        sf::Vector2i cell = block.getCell(i);
        if (cell.x < 0 || cell.x >= 10 || cell.y >= 20 || grid[cell.y][cell.x])
            return true;
    }
    return false;
}

void Grid::placeBlock(const Block& block) {
    for (int i = 0; i < 4; ++i) {
        sf::Vector2i cell = block.getCell(i);
        grid[cell.y][cell.x] = block.getColorID();
    }
}

int Grid::clearLines() {
    int linesCleared = 0;
    for (int y = 19; y >= 0; --y) {
        bool full = true;
        for (int x = 0; x < 10; ++x)
            if (!grid[y][x])
                full = false;

        if (full) {
            ++linesCleared; // Count this line
            for (int i = y; i > 0; --i)
                for (int j = 0; j < 10; ++j)
                    grid[i][j] = grid[i - 1][j]; // Shift upper rows down
            for (int j = 0; j < 10; ++j)
                grid[0][j] = 0; // Clear the top row
            ++y; // Recheck the same row
        }
    }
    return linesCleared;
}

int Grid::getCell(int row, int col) const {
    return grid[row][col];
}
