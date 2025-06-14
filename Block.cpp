#include "Block.h"

// Define all block shapes (7 types, each with 4 cells)
const sf::Vector2i SHAPES[7][4] = {
    { {0,1}, {1,1}, {2,1}, {3,1} }, // I
    { {1,0}, {2,0}, {1,1}, {2,1} }, // O
    { {1,0}, {2,0}, {0,1}, {1,1} }, // S
    { {0,0}, {1,0}, {1,1}, {2,1} }, // Z
    { {1,0}, {0,1}, {1,1}, {2,1} }, // T
    { {0,0}, {0,1}, {1,1}, {2,1} }, // J
    { {2,0}, {0,1}, {1,1}, {2,1} }  // L
};

Block::Block() {
    setShape(rand() % 7);
}

void Block::setShape(int shapeID) {
    shape = shapeID;
    color = shapeID + 1;
    for (int i = 0; i < 4; ++i)
        cells[i] = SHAPES[shape][i];
    position = sf::Vector2i(3, 0); // start position (x, y)
}

void Block::move(int dx, int dy) {
    position.x += dx; // gives the  horizontal movement
    position.y += dy; // gives the vertical movement
}

void Block::rotate() {
    if (shape == 1) return; // O shape doesn't rotate
    for (int i = 0; i < 4; ++i) {
        int x = cells[i].x;
        int y = cells[i].y;
        cells[i].x = -y; // shifting to 90 degree
        cells[i].y = x; // shifting to 90 degrees
    }
}

sf::Vector2i Block::getCell(int i) const {
    return sf::Vector2i(position.x + cells[i].x, position.y + cells[i].y);// to return the exact cell of block
}

int Block::getColorID() const {
    return color;
}
