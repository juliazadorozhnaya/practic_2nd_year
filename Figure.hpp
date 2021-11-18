#pragma once

#include <string>

// цвет фигуры
enum class Color {
    Black, // чёрный
    White // белый
};

// клетка
struct Point {
    int x;
    int y;
};

// фигура
class Figure {
protected:
    Color color; // цвет
    Point point; // текущая клетка
public:
    Figure(Color color, Point point); // конструктор фигуры

    bool IsEat(const Figure& figure) const;
    bool IsAtPoint(Point p) const;
    bool IsSameColor(const Figure& figure) const;
    virtual bool Move(Point p, Figure *figure) = 0; // чисто виртуальный метод хода
    virtual void Show() const = 0; // чисто виртуальный метод вывода фигуры
};

// король
class King : public Figure {
public:
    King(Color color, Point point); // конструктор короля
    bool Move(Point p, Figure *figure); // ход
    void Show() const; // вывод фигуры
};

// ферзь
class Queen : public Figure {
public:
    Queen(Color color, Point point); // конструктор ферзя
    bool Move(Point p, Figure *figure); // ход
    void Show() const; // вывод фигуры
};

// ладья
class Rook : public Figure {
public:
    Rook(Color color, Point point); // конструктор ладьи
    bool Move(Point p, Figure *figure); // ход
    void Show() const; // вывод фигуры
};

// слон
class Bishop : public Figure {
public:
    Bishop(Color color, Point point); // конструктор слона
    bool Move(Point p, Figure *figure); // ход
    void Show() const; // вывод фигуры
};

// коня
class Knight : public Figure {
public:
    Knight(Color color, Point point); // конструктор коня
    bool Move(Point p, Figure *figure); // ход
    void Show() const; // вывод фигуры
};

// знак
int Sign(int value) {
    if (value > 0)
        return 1;

    if (value < 0)
        return -1;

    return 0;
}

// конструктор фигуры
Figure::Figure(Color color, Point point) {
    this->color = color;
    this->point = point;
}

bool Figure::IsEat(const Figure& figure) const {
    return color != figure.color && point.x == figure.point.x && point.y == figure.point.y;
}

bool Figure::IsAtPoint(Point p) const {
    return point.x == p.x && point.y == p.y;
}

bool Figure::IsSameColor(const Figure& figure) const {
    return color == figure.color;
}

// конструктор короля
King::King(Color color, Point point) : Figure(color, point) {
}

// конструктор ферзя
Queen::Queen(Color color, Point point) : Figure(color, point) {
}

// конструктор ладьи
Rook::Rook(Color color, Point point) : Figure(color, point) {
}

// конструктор слона
Bishop::Bishop(Color color, Point point) : Figure(color, point) {
}

// конструктор коня
Knight::Knight(Color color, Point point) : Figure(color, point) {
}

// ход короля
bool King::Move(Point p, Figure *figure) {
    if (p.x < 0 || p.y < 0 || p.x >= 8 || p.y >= 8) // если ход за пределы доски
        return false; // то нельзя сделать ход

    if (p.x == point.x && p.y == point.y)
        return false; // запрещаем ход в ту же клетку

    int dx = abs(p.x - point.x);
    int dy = abs(p.y - point.y);

    if (dx != 1 && dy != 1) // если нелья сделать ход
        return false; // то возвращаем ложь

    if (figure && figure->IsAtPoint(p) && IsSameColor(*figure))
        return false;

    point = p; // делаем ход
    return true;
}

// вывод фигуры
void King::Show() const {
    if (color == Color::White)
        std::cout << "белый";
    else
        std::cout << "чёрный";
    
    std::cout << " король на " << (char)('a' + point.y) << (char)('1' + point.x) << std::endl;
}

// ход ферзя
bool Queen::Move(Point p, Figure *figure) {
    if (p.x < 0 || p.y < 0 || p.x >= 8 || p.y >= 8) // если ход за пределы доски
        return false; // то нельзя сделать ход

    if (p.x == point.x && p.y == point.y)
        return false; // запрещаем ход в ту же клетку

    int dx = p.x - point.x;
    int dy = p.y - point.y;

    if (dx != 0 && dy != 0 && abs(dx) != abs(dy)) // если нелья сделать ход
        return false; // то возвращаем ложь

    Point curr = point;

    while (figure && (curr.x != p.x || curr.y != p.y)) {
        if (figure->IsAtPoint(curr))
            return false;

        curr.x += Sign(dx);
        curr.y += Sign(dy);
    }

    point = p; // делаем ход
    return true;
}

// вывод фигуры
void Queen::Show() const {
    if (color == Color::White)
        std::cout << "белый";
    else
        std::cout << "чёрный";
    
    std::cout << " ферзь на " << (char)('a' + point.y) << (char)('1' + point.x) << std::endl;
}

// ход ладьи
bool Rook::Move(Point p, Figure *figure) {
    if (p.x < 0 || p.y < 0 || p.x >= 8 || p.y >= 8) // если ход за пределы доски
        return false; // то нельзя сделать ход

    if (p.x == point.x && p.y == point.y)
        return false; // запрещаем ход в ту же клетку

    int dx = p.x - point.x;
    int dy = p.y - point.y;

    if (dx != 0 && dy != 0) // если нелья сделать ход
        return false; // то возвращаем ложь

    Point curr = point;

    while (figure && (curr.x != p.x || curr.y != p.y)) {
        if (figure->IsAtPoint(curr))
            return false;

        curr.x += Sign(dx);
        curr.y += Sign(dy);
    }

    point = p; // делаем ход
    return true;
}

// вывод фигуры
void Rook::Show() const {
    if (color == Color::White)
        std::cout << "белая";
    else
        std::cout << "чёрная";
    
    std::cout << " ладья на " << (char)('a' + point.y) << (char)('1' + point.x) << std::endl;
}

// ход слона
bool Bishop::Move(Point p, Figure *figure) {
    if (p.x < 0 || p.y < 0 || p.x >= 8 || p.y >= 8) // если ход за пределы доски
        return false; // то нельзя сделать ход

    if (p.x == point.x && p.y == point.y)
        return false; // запрещаем ход в ту же клетку

    int dx = p.x - point.x;
    int dy = p.y - point.y;

    if (abs(dx) != abs(dy)) // если нелья сделать ход
        return false; // то возвращаем ложь

    Point curr = point;

    while (figure && (curr.x != p.x || curr.y != p.y)) {
        if (figure->IsAtPoint(curr))
            return false;

        curr.x += Sign(dx);
        curr.y += Sign(dy);
    }

    point = p; // делаем ход
    return true;
}

// вывод фигуры
void Bishop::Show() const {
    if (color == Color::White)
        std::cout << "белый";
    else
        std::cout << "чёрный";
    
    std::cout << " слон на " << (char)('a' + point.y) << (char)('1' + point.x) << std::endl;
}

// ход коня
bool Knight::Move(Point p, Figure *figure) {
    if (p.x < 0 || p.y < 0 || p.x >= 8 || p.y >= 8) // если ход за пределы доски
        return false; // то нельзя сделать ход

    if (p.x == point.x && p.y == point.y)
        return false; // запрещаем ход в ту же клетку

    int dx = abs(p.x - point.x);
    int dy = abs(p.y - point.y);

    if (dx + dy != 3 || dx == 0 || dy == 0) // если нелья сделать ход
        return false; // то возвращаем ложь

    if (figure && figure->IsAtPoint(p) && IsSameColor(*figure))
        return false;

    point = p; // делаем ход
    return true;
}

// вывод фигуры
void Knight::Show() const {
    if (color == Color::White)
        std::cout << "белый";
    else
        std::cout << "чёрный";
    
    std::cout << " конь на " << (char)('a' + point.y) << (char)('1' + point.x) << std::endl;
}

// перевод клетки в координату
Point CellToPoint(const std::string& cell) {
    Point point;
    point.x = cell[1] - '1';
    point.y = cell[0] - 'a';
    return point; // возвраащем клетку
}