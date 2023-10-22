// Программа для реализации игры жизнь в стиле ООП
// Использует библиотеку SFML для графического интерфейса
// Компилировать с флагами -lsfml-graphics -lsfml-window -lsfml-system

#include <SFML/Graphics.hpp>
#include <vector>
#include <iostream>

// Константы для размера окна, клеток и количества клеток
const int WINDOW_WIDTH = 800;
const int WINDOW_HEIGHT = 600;
const int CELL_SIZE = 10;
const int CELL_COUNT_X = WINDOW_WIDTH / CELL_SIZE;
const int CELL_COUNT_Y = WINDOW_HEIGHT / CELL_SIZE;

// Класс для представления одной клетки
class Cell {
private:
    bool alive; // Состояние клетки (живая или мертвая)
    sf::RectangleShape shape; // Форма клетки для отрисовки
public:
    // Конструктор по умолчанию
    Cell() {
        alive = false; // Изначально клетка мертва
        shape.setSize(sf::Vector2f(CELL_SIZE, CELL_SIZE)); // Устанавливаем размер формы
        shape.setFillColor(sf::Color::Black); // Устанавливаем цвет формы (черный для мертвой клетки)
        shape.setOutlineColor(sf::Color::White); // Устанавливаем цвет обводки формы (белый для всех клеток)
        shape.setOutlineThickness(1); // Устанавливаем толщину обводки формы
    }

    // Метод для установки состояния клетки
    void setAlive(bool state) {
        alive = state; // Запоминаем состояние
        if (alive) { // Если клетка живая
            shape.setFillColor(sf::Color::Green); // Устанавливаем цвет формы в зеленый
        }
        else { // Если клетка мертвая
            shape.setFillColor(sf::Color::Black); // Устанавливаем цвет формы в черный
        }
    }

    // Метод для получения состояния клетки
    bool isAlive() {
        return alive; // Возвращаем состояние
    }

    // Метод для установки позиции клетки на экране
    void setPosition(int x, int y) {
        shape.setPosition(x * CELL_SIZE, y * CELL_SIZE); // Устанавливаем позицию формы в зависимости от координат клетки и размера клетки
    }

    // Метод для отрисовки клетки на окне
    void draw(sf::RenderWindow& window) {
        window.draw(shape); // Рисуем форму на окне
    }
};

// Класс для представления всего поля клеток
class Field {
private:
    std::vector<std::vector<Cell>> cells; // Двумерный вектор клеток
public:
    // Конструктор по умолчанию
    Field() {
        cells.resize(CELL_COUNT_X); // Изменяем размер вектора по горизонтали
        for (int i = 0; i < CELL_COUNT_X; i++) {
            cells[i].resize(CELL_COUNT_Y); // Изменяем размер вектора по вертикали
            for (int j = 0; j < CELL_COUNT_Y; j++) {
                cells[i][j].setPosition(i, j); // Устанавливаем позицию каждой клетки на экране
            }
        }
    }

    // Метод для установки состояния клетки по координатам
    void setCell(int x, int y, bool state) {
        if (x >= 0 && x < CELL_COUNT_X && y >= 0 && y < CELL_COUNT_Y) { // Проверяем, что координаты в пределах поля
            cells[x][y].setAlive(state); // Устанавливаем состояние клетки
        }
    }

    // Метод для получения состояния клетки по координатам
    bool getCell(int x, int y) {
        if (x >= 0 && x < CELL_COUNT_X && y >= 0 && y < CELL_COUNT_Y) { // Проверяем, что координаты в пределах поля
            return cells[x][y].isAlive(); // Возвращаем состояние клетки
        }
        else { // Если координаты за пределами поля
            return false; // Считаем, что такая клетка мертва
        }
    }

    // Метод для подсчета количества живых соседей у клетки по координатам
    int countNeighbors(int x, int y) {
        int count = 0; // Счетчик живых соседей
        for (int i = -1; i <= 1; i++) { // Проходим по соседним строкам
            for (int j = -1; j <= 1; j++) { // Проходим по соседним столбцам
                if (i == 0 && j == 0) continue; // Пропускаем саму клетку
                if (getCell(x + i, y + j)) count++; // Если соседняя клетка живая, увеличиваем счетчик
            }
        }
        return count; // Возвращаем счетчик
    }

    // Метод для обновления состояния всего поля по правилам игры жизнь
    void update() {
        std::vector<std::vector<bool>> newStates(CELL_COUNT_X, std::vector<bool>(CELL_COUNT_Y)); // Создаем двумерный вектор для хранения новых состояний клеток
        for (int i = 0; i < CELL_COUNT_X; i++) { // Проходим по всем строкам поля
            for (int j = 0; j < CELL_COUNT_Y; j++) { // Проходим по всем столбцам поля
                int n = countNeighbors(i, j); // Считаем количество живых соседей у клетки
                if (getCell(i, j)) { // Если клетка живая
                    if (n == 2 || n == 3) { // Если у нее два или три живых соседа
                        newStates[i][j] = true; // Она остается живой в следующем поколении
                    }
                    else { // Если у нее меньше двух или больше трех живых соседей
                        newStates[i][j] = false; // Она умирает в следующем поколении
                    }
                }
                else { // Если клетка мертвая
                    if (n == 3) { // Если у нее ровно три живых соседа
                        newStates[i][j] = true; // Она оживает в следующем поколении
                    }
                    else { // Если у нее не три живых соседа
                        newStates[i][j] = false; // Она остается мертвой в следующем поколении
                    }
                }
            }
        }
        for (int i = 0; i < CELL_COUNT_X; i++) { // Проходим по всем строкам поля
            for (int j = 0; j < CELL_COUNT_Y; j++) { // Проходим по всем столбцам поля
                setCell(i, j, newStates[i][j]); // Устанавливаем новое состояние каждой клетки
            }
        }
    }

    // Метод для отрисовки всего поля на окне
    void draw(sf::RenderWindow& window) {
        for (int i = 0; i < CELL_COUNT_X; i++) { // Проходим по всем строкам поля
            for (int j = 0; j < CELL_COUNT_Y; j++) { // Проходим по всем столбцам поля
                cells[i][j].draw(window); // Рисуем каждую клетку на окне
            }
        }
    }
};

// Класс для представления игры жизнь как целого объекта
class Game {
private:
    Field field; // Поле клеток
    sf::RenderWindow window; // Окно для отрисовки
    sf::Clock clock; // Таймер для контроля скорости смены поколений
    float delay; // Задержка между поколениями в секундах
    bool paused; // Флаг для приостановки игры
public:
    // Конструктор по умолчанию
    Game() {
        window.create(sf::VideoMode(WINDOW_WIDTH, WINDOW_HEIGHT), "Game of Life"); // Создаем окно с заданными размерами и заголовком
        window.setFramerateLimit(60); // Устанавливаем ограничение на частоту кадров
        delay = 0.1; // Устанавливаем начальную задержку в 0.1 секунды
        paused = false; // Изначально игра не приостановлена
    }

    // Метод для запуска игры
    void run() {
        while (window.isOpen()) { // Пока окно открыто
            handleEvents(); // Обрабатываем события
            update(); // Обновляем состояние игры
            render(); // Отрисовываем игру на экране
        }
    }

    // Метод для обработки событий
    void handleEvents() {
        sf::Event event; // Переменная для хранения события
        while (window.pollEvent(event)) { // Пока есть события в очереди
            switch (event.type) { // В зависимости от типа события
            case sf::Event::Closed: // Если событие закрытия окна
                window.close(); // Закрываем окно
                break;
            case sf::Event::KeyPressed: // Если событие нажатия клавиши
                handleKeyboard(event.key.code); // Обрабатываем клавиатуру
                break;
            case sf::Event::MouseButtonPressed: // Если событие нажатия кнопки мыши
                handleMouse(event.mouseButton.button, event.mouseButton.x, event.mouseButton.y); // Обрабатываем мышь
                break;
            default: // Для других типов событий
                break;
            }
        }
    }

    // Метод для обработки клавиатуры
    void handleKeyboard(sf::Keyboard::Key key) {
        switch (key) { // В зависимости от кода клавиши
        case sf::Keyboard::Escape: // Если клавиша Escape
            window.close(); // Закрываем окно
            break;
        case sf::Keyboard::Space: // Если клавиша Space
            paused = !paused; // Переключаем флаг приостановки игры на противоположный
            break;
        case sf::Keyboard::Up: // Если клавиша Up
            delay -= 0.01; // Уменьшаем задержку на 0.01 секунды
            if (delay < 0) delay = 0; // Не даем задержке быть отрицательной
            break;
        case sf::Keyboard::Down: // Если клавиша Down
            delay += 0.01; // Увеличиваем задержку на 0.01 секунды
            break;
        default: // Для других клавиш
            break;
        }
    }

    // Метод для обработки мыши
    void handleMouse(sf::Mouse::Button button, int x, int y) {
        if (button == sf::Mouse::Left) { // Если кнопка левая
            int i = x / CELL_SIZE; // Вычисляем координату клетки по горизонтали
            int j = y / CELL_SIZE; // Вычисляем координату клетки по вертикали
            field.setCell(i, j, true); // Устанавливаем состояние клетки в живое
        }
    }

    // Метод для обновления состояния игры
    void update() {
        if (!paused) { // Если игра не приостановлена
            if (clock.getElapsedTime().asSeconds() > delay) { // Если прошло больше времени, чем задержка
                field.update(); // Обновляем состояние поля
                clock.restart(); // Перезапускаем таймер
            }
        }
    }

    // Метод для отрисовки игры на экране
    void render() {
        window.clear(); // Очищаем окно
        field.draw(window); // Рисуем поле на окне
        window.display(); // Отображаем окно
    }
};

// Главная функция программы
int main() {
    Game game; // Создаем объект игры
    game.run(); // Запускаем игру
    return 0; // Возвращаем нулевой код завершения
}