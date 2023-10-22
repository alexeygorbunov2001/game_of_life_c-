// ��������� ��� ���������� ���� ����� � ����� ���
// ���������� ���������� SFML ��� ������������ ����������
// ������������� � ������� -lsfml-graphics -lsfml-window -lsfml-system

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

// ��������� ��� ������� ����, ������ � ���������� ������
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = 10;
const int CELL_COUNT_X = WINDOW_WIDTH / CELL_SIZE;
const int CELL_COUNT_Y = WINDOW_HEIGHT / CELL_SIZE;

// ����� ��� ������������� ����� ������
class Cell {
private:
    bool alive; // ��������� ������ (����� ��� �������)
    sf::RectangleShape shape; // ����� ������ ��� ���������
public:
    // ����������� �� ���������
    Cell() {
        alive = false; // ���������� ������ ������
        shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE)); // ������������� ������ �����
        shape.setFillColor(sf::Color::Black); // ������������� ���� ����� (������ ��� ������� ������)
        shape.setOutlineColor(sf::Color::White); // ������������� ���� ������� ����� (����� ��� ���� ������)
        shape.setOutlineThickness(1); // ������������� ������� ������� �����
    }

    // ����� ��� ��������� ��������� ������
    void setAlive(bool state) {
        alive = state; // ���������� ���������
        if (alive) { // ���� ������ �����
            shape.setFillColor(sf::Color::Green); // ������������� ���� ����� � �������
        }
        else { // ���� ������ �������
            shape.setFillColor(sf::Color::Black); // ������������� ���� ����� � ������
        }
    }

    // ����� ��� ��������� ��������� ������
    bool isAlive() {
        return alive; // ���������� ���������
    }

    // ����� ��� ��������� ������� ������ �� ������
    void setPosition(int x, int y) {
        shape.setPosition(x * CELL_SIZE, y * CELL_SIZE); // ������������� ������� ����� � ����������� �� ��������� ������ � ������� ������
    }

    // ����� ��� ��������� ������ �� ����
    void draw(sf::RenderWindow& window) {
        window.draw(shape); // ������ ����� �� ����
    }
};

// ����� ��� ������������� ����� ���� ������
class Field {
private:
    std::vector<std::vector<Cell>> cells; // ��������� ������ ������
public:
    // ����������� �� ���������
    Field() {
        cells.resize(CELL_COUNT_X); // �������� ������ ������� �� �����������
        for (int i = 0; i < CELL_COUNT_X; i++) {
            cells[i].resize(CELL_COUNT_Y); // �������� ������ ������� �� ���������
            for (int j = 0; j < CELL_COUNT_Y; j++) {
                cells[i][j].setPosition(i, j); // ������������� ������� ������ ������ �� ������
            }
        }
    }

    // ����� ��� ��������� ��������� ������ �� �����������
    void setCell(int x, int y, bool state) {
        if (x >= 0 && x < CELL_COUNT_X && y >= 0 && y < CELL_COUNT_Y) { // ���������, ��� ���������� � �������� ����
            cells[x][y].setAlive(state); // ������������� ��������� ������
        }
    }

    // ����� ��� ��������� ��������� ������ �� �����������
    bool getCell(int x, int y) {
        if (x >= 0 && x < CELL_COUNT_X && y >= 0 && y < CELL_COUNT_Y) { // ���������, ��� ���������� � �������� ����
            return cells[x][y].isAlive(); // ���������� ��������� ������
        }
        else { // ���� ���������� �� ��������� ����
            return false; // �������, ��� ����� ������ ������
        }
    }

    // ����� ��� �������� ���������� ����� ������� � ������ �� �����������
    int countNeighbors(int x, int y) {
        int count = 0; // ������� ����� �������
        for (int i = -1; i <= 1; i++) { // �������� �� �������� �������
            for (int j = -1; j <= 1; j++) { // �������� �� �������� ��������
                if (i == 0 && j == 0) continue; // ���������� ���� ������
                if (getCell(x + i, y + j)) count++; // ���� �������� ������ �����, ����������� �������
            }
        }
        return count; // ���������� �������
    }

    // ����� ��� ���������� ��������� ����� ���� �� �������� ���� �����
    void update() {
        std::vector<std::vector<bool>> newStates(CELL_COUNT_X, std::vector<bool>(CELL_COUNT_Y)); // ������� ��������� ������ ��� �������� ����� ��������� ������
        for (int i = 0; i < CELL_COUNT_X; i++) { // �������� �� ���� ������� ����
            for (int j = 0; j < CELL_COUNT_Y; j++) { // �������� �� ���� �������� ����
                int n = countNeighbors(i, j); // ������� ���������� ����� ������� � ������
                if (getCell(i, j)) { // ���� ������ �����
                    if (n == 2 || n == 3) { // ���� � ��� ��� ��� ��� ����� ������
                        newStates[i][j] = true; // ��� �������� ����� � ��������� ���������
                    }
                    else { // ���� � ��� ������ ���� ��� ������ ���� ����� �������
                        newStates[i][j] = false; // ��� ������� � ��������� ���������
                    }
                }
                else { // ���� ������ �������
                    if (n == 3) { // ���� � ��� ����� ��� ����� ������
                        newStates[i][j] = true; // ��� ������� � ��������� ���������
                    }
                    else { // ���� � ��� �� ��� ����� ������
                        newStates[i][j] = false; // ��� �������� ������� � ��������� ���������
                    }
                }
            }
        }
        for (int i = 0; i < CELL_COUNT_X; i++) { // �������� �� ���� ������� ����
            for (int j = 0; j < CELL_COUNT_Y; j++) { // �������� �� ���� �������� ����
                setCell(i, j, newStates[i][j]); // ������������� ����� ��������� ������ ������
            }
        }
    }

    // ����� ��� ��������� ����� ���� �� ����
    void draw(sf::RenderWindow& window) {
        for (int i = 0; i < CELL_COUNT_X; i++) { // �������� �� ���� ������� ����
            for (int j = 0; j < CELL_COUNT_Y; j++) { // �������� �� ���� �������� ����
                cells[i][j].draw(window); // ������ ������ ������ �� ����
            }
        }
    }
};

// ����� ��� ������������� ���� ����� ��� ������ �������
class Game {
private:
    Field field; // ���� ������
    sf::RenderWindow window; // ���� ��� ���������
    sf::Clock clock; // ������ ��� �������� �������� ����� ���������
    float delay; // �������� ����� ����������� � ��������
    bool paused; // ���� ��� ������������ ����
public:
    // ����������� �� ���������
    Game() {
        window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game of Life"); // ������� ���� � ��������� ��������� � ����������
        window.setFramerateLimit(60); // ������������� ����������� �� ������� ������
        delay = 0.1; // ������������� ��������� �������� � 0.1 �������
        paused = false; // ���������� ���� �� ��������������
    }

    // ����� ��� ������� ����
    void run() {
        while (window.isOpen()) { // ���� ���� �������
            handleEvents(); // ������������ �������
            update(); // ��������� ��������� ����
            render(); // ������������ ���� �� ������
        }
    }

    // ����� ��� ��������� �������
    void handleEvents() {
        sf::Event event; // ���������� ��� �������� �������
        while (window.pollEvent(event)) { // ���� ���� ������� � �������
            switch (event.type) { // � ����������� �� ���� �������
            case sf::Event::Closed: // ���� ������� �������� ����
                window.close(); // ��������� ����
                break;
            case sf::Event::KeyPressed: // ���� ������� ������� �������
                handleKeyboard(event.key.code); // ������������ ����������
                break;
            case sf::Event::MouseButtonPressed: // ���� ������� ������� ������ ����
                handleMouse(event.mouseButton.button, event.mouseButton.x, event.mouseButton.y); // ������������ ����
                break;
            default: // ��� ������ ����� �������
                break;
            }
        }
    }

    // ����� ��� ��������� ����������
    void handleKeyboard(sf::Keyboard::Key key) {
        switch (key) { // � ����������� �� ���� �������
        case sf::Keyboard::Escape: // ���� ������� Escape
            window.close(); // ��������� ����
            break;
        case sf::Keyboard::Space: // ���� ������� Space
            paused = !paused; // ����������� ���� ������������ ���� �� ���������������
            break;
        case sf::Keyboard::Up: // ���� ������� Up
            delay -= 0.01; // ��������� �������� �� 0.01 �������
            if (delay < 0) delay = 0; // �� ���� �������� ���� �������������
            break;
        case sf::Keyboard::Down: // ���� ������� Down
            delay += 0.01; // ����������� �������� �� 0.01 �������
            break;
        default: // ��� ������ ������
            break;
        }
    }

    // ����� ��� ��������� ����
    void handleMouse(sf::Mouse::Button button, int x, int y) {
        if (button == sf::Mouse::Left) { // ���� ������ �����
            int i = x / CELL_SIZE; // ��������� ���������� ������ �� �����������
            int j = y / CELL_SIZE; // ��������� ���������� ������ �� ���������
            field.setCell(i, j, true); // ������������� ��������� ������ � �����
        }
    }

    // ����� ��� ���������� ��������� ����
    void update() {
        if (!paused) { // ���� ���� �� ��������������
            if (clock.getElapsedTime().asSeconds() > delay) { // ���� ������ ������ �������, ��� ��������
                field.update(); // ��������� ��������� ����
                clock.restart(); // ������������� ������
            }
        }
    }

    // ����� ��� ��������� ���� �� ������
    void render() {
        window.clear(); // ������� ����
        field.draw(window); // ������ ���� �� ����
        window.display(); // ���������� ����
    }
};

// ������� ������� ���������
int main() {
    Game game; // ������� ������ ����
    game.run(); // ��������� ����
    return 0; // ���������� ������� ��� ����������
}