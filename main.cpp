#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include "Block.h"
#include "Grid.h"
#include "colors.h" // Added for centralized colors

const int BLOCK_SIZE = 35;
bool isDownPressed = false;// to check whether the down button is pressed or not

void drawBackground(sf::RenderWindow& window, sf::Sprite& backgroundSprite) {
    window.draw(backgroundSprite);
}

int main() {
    int windowWidth = 10 * BLOCK_SIZE + 150;
    int windowHeight = 20 * BLOCK_SIZE;

    sf::RenderWindow window(sf::VideoMode(windowWidth, windowHeight), "Tetris SFML with Score");
    window.setFramerateLimit(60);
    // background texture
    sf::Texture backgroundTexture;
    if (!backgroundTexture.loadFromFile("background2.png")) return -1;
    sf::Sprite backgroundSprite(backgroundTexture);
    backgroundSprite.setScale(
        (float)window.getSize().x / backgroundTexture.getSize().x,
        (float)window.getSize().y / backgroundTexture.getSize().y);
    // text font loading
    sf::Font font;
    if (!font.loadFromFile("arial.ttf")) return -1;
    // score font
    sf::Text scoreText("", font, 24);
    scoreText.setFillColor(sf::Color::White);
    scoreText.setPosition(10 * BLOCK_SIZE + 20, 20);
    // clear lines font
    sf::Text ClearLines("", font, 24);
    ClearLines.setFillColor(sf::Color::White);
    ClearLines.setPosition(8 * BLOCK_SIZE + 20, 60);
    // next block font
    sf::Text nextBlockLabel("Next:", font, 24);
    nextBlockLabel.setFillColor(sf::Color::White);
    nextBlockLabel.setPosition(10 * BLOCK_SIZE + 20, 100);

    // level 2 font

    sf::Text level2("LEVEL 2", font, 50);
    level2.setFillColor(sf::Color::Red);
    level2.setPosition(windowWidth / 4, windowHeight / 2);

    // level 3 font

    sf::Text level3("LEVEL 3", font, 50);
    level3.setFillColor(sf::Color::Red);
    level3.setPosition(windowWidth / 4, windowHeight / 2);

    bool showLevel2 = false;// level 2 font is not shown while it is false
    bool showlevel3 = false;// level 3 font is not shown while it is false
    bool level2Shown = false;
    bool level3shown = false;
    sf::Clock level2Clock;
    sf::Clock level3Clock;

    int score = 0, clearedlines = 0;

    Grid grid;
    Block current;
    Block next;
    next.setShape(rand() % 7);

    sf::Clock clock;
    float fallSpeed = 0.05f, fallTimer = 0;

    bool gameOver = false;

    // Background music
    sf::Music backgroundMusic;
    if (!backgroundMusic.openFromFile("backgroundmusic.wav")) return -1;
    backgroundMusic.setLoop(true);
    backgroundMusic.play();

    // Game over sound
    sf::SoundBuffer gameOverBuffer;
    if (!gameOverBuffer.loadFromFile("gameover.mp3")) return -1;
    sf::Sound gameOverSound;
    gameOverSound.setBuffer(gameOverBuffer);
    bool gameOverSoundPlayed = false;

    while (window.isOpen()) {
        if (gameOver) {
            if (!gameOverSoundPlayed) {  //condition for gameover music
                backgroundMusic.stop();
                gameOverSound.play();
                gameOverSoundPlayed = true;
            }
            // Game over font
            sf::Text gameOverText("Game Over", font, 50);
            gameOverText.setFillColor(sf::Color::Red);
            gameOverText.setPosition(windowWidth / 4, windowHeight / 2);
            window.clear();
            drawBackground(window, backgroundSprite);
            window.draw(gameOverText);
            window.display();
            continue;// game will come out from the main loop
        }

        float time = clock.restart().asSeconds();
        fallTimer += time;

        sf::Event event;
        while (window.pollEvent(event)) {  // Keyboard set up
            if (event.type == sf::Event::Closed) window.close();
            else if (event.type == sf::Event::KeyPressed) {
                if (event.key.code == sf::Keyboard::Left) {
                    current.move(-1, 0); // towards - x-axis
                    if (grid.checkCollision(current)) current.move(1, 0); // condition for left collision
                }
                else if (event.key.code == sf::Keyboard::Right) {
                    current.move(1, 0); // toward + x-axis
                    if (grid.checkCollision(current)) current.move(-1, 0); // condition for left collision
                }
                else if (event.key.code == sf::Keyboard::Up) {
                    current.rotate();
                    if (grid.checkCollision(current)) {
                        current.rotate(); current.rotate(); current.rotate();
                    }
                }
                else if (event.key.code == sf::Keyboard::Down) {
                    isDownPressed = true;
                }
            }
            else if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Down) {
                    isDownPressed = false;
                }
            }
        }

        if (score >= 100) { // Condition for level 2 for increasing speed
            fallSpeed = 0.3f; // Level 2 speed
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Down) {
                    isDownPressed = false; // Stop fast drop when released
                }

            }
            if (isDownPressed) {
                fallSpeed = 0.05f; // Fast fall when Down is pressed
            }
        }
        if (score >= 200) { // Condition for level 2 for increasing speed
            fallSpeed = 0.2f; // Level 3 speed
            if (event.type == sf::Event::KeyReleased) {
                if (event.key.code == sf::Keyboard::Down) {
                    isDownPressed = false; // Stop fast drop when released
                }

            }
            if (isDownPressed) {
                fallSpeed = 0.05f; // Fast fall when Down is pressed
            }
        }
        else if (isDownPressed) {
            fallSpeed = 0.05f; // Fast fall when Down is pressed
        }
        else {
            fallSpeed = 0.5f; // Normal speed
        }

        if (fallTimer >= fallSpeed) {
            current.move(0, 1);
            if (grid.checkCollision(current)) {
                current.move(0, -1);
                grid.placeBlock(current);
                int cleared = grid.clearLines();
                score += cleared * 100; // formula for score
                clearedlines += cleared; // formula for cleared lines
                current = next;
                next.setShape(rand() % 7);
                if (grid.checkCollision(current)) gameOver = true;
                fallSpeed = 0.5f;
            }
            fallTimer = 0;
        }
        // Level 2 Clock
        if (score >= 100 && !level2Shown) {
            showLevel2 = true;
            level2Clock.restart();
            level2Shown = true;
        }
        // level 3 Clock
        if (score >= 200 && !level3shown) {
            showlevel3 = true;
            level3Clock.restart();
            level3shown = true;
        }
        if (showLevel2 && level2Clock.getElapsedTime().asSeconds() > 3) { // checking the time of 3sec
            showLevel2 = false;
        }
        if (showlevel3 && level3Clock.getElapsedTime().asSeconds() > 3) { // checking the time of 3sec
            showlevel3 = false;
        }

        scoreText.setString("Score: " + std::to_string(score)); // updating score
        ClearLines.setString("Clear Lines: " + std::to_string(clearedlines)); // updating cleared lines

        window.clear();
        drawBackground(window, backgroundSprite);
        grid.draw(window);

        sf::RectangleShape rect(sf::Vector2f(BLOCK_SIZE, BLOCK_SIZE));
        rect.setFillColor(COLORS[current.getColorID()]);// selecting the color for the rectangle
        for (int i = 0; i < 4; ++i) { // each tetrus is of 4 blocks
            sf::Vector2i cell = current.getCell(i);
            rect.setPosition(cell.x * BLOCK_SIZE, cell.y * BLOCK_SIZE);// converting it into pixels
            window.draw(rect);
        }

        rect.setSize(sf::Vector2f(BLOCK_SIZE / 2, BLOCK_SIZE / 2)); // for small size
        rect.setFillColor(COLORS[next.getColorID()]);
        for (int i = 0; i < 4; ++i) {
            sf::Vector2i cell = next.getCell(i);
            float previewX = 10 * BLOCK_SIZE + 40 + (cell.x - 3) * BLOCK_SIZE / 2; // doing this to position the block outside the grid
            float previewY = 140 + (cell.y - 1) * BLOCK_SIZE / 2;
            rect.setPosition(previewX, previewY);
            window.draw(rect);
        }

        window.draw(scoreText);
        window.draw(ClearLines);
        window.draw(nextBlockLabel);

        if (showLevel2) {
            window.draw(level2);
        }
        if (showlevel3) {
            window.draw(level3);
        }

        window.display();
    }

    return 0;
}
