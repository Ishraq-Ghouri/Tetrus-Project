
#pragma once

#include <SFML/Graphics.hpp>

class Block {
private:
    sf::Vector2i cells[4];
    sf::Vector2i position;
    int shape;
    int color;
public:
    Block();
    void setShape(int shapeID);
    void move(int dx, int dy);
    void rotate();
    sf::Vector2i getCell(int i) const;
    int getColorID() const;
};
