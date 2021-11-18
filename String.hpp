#pragma once
#include <iostream>

class StringIndexErrorException {
public:
    StringIndexErrorException();
};

class StringOutOfMemoryException {
public:
    StringOutOfMemoryException();
};

class StringCountErrorException {
public:
    StringCountErrorException();
};

class String {
    char *chars; // массив символов
    int size; // длина строки

    static int stringCount;
public:

    String(); // конструктор по умолчанию
    String(const String& string); // конструктор копирования
    String(const char *s); // конструктор из Си-строки

    String& operator=(const String& string); // оператор присваивания

    int GetLength() const; // получение длины
    bool IsEmpty() const; // пуста ли строка

    void Add(char c); // добавление символа в конец строки
    void Add(const String& string); // добавление строки в конец строки

    void Insert(char c, int index); // вставка символа по индексу
    void Insert(const String& string, int index); // вставка строки по индексу

    void Remove(int index); // удаление символа по индексу
    void Remove(int start, int end); // удаление подстроки из интервала

    int IndexOf(const String& string) const; // индекс вхождения подстроки
    void Replace(const String& substring, const String& replacement); // замена подстроки на строку

    String operator+(const String& string) const; // соединение двух строк
    String operator*(int count) const; // умножение строки на число

    bool operator==(const String& string) const; // сравнение на равенство
    bool operator<(const String& string) const; // сравнение на меньше
    bool operator>(const String& string) const; // сравнение на больше
    bool operator<=(const String& string) const; // сравнение на меньше или равно
    bool operator>=(const String& string) const; // сравнение на больше или равно

    char operator[](int index) const; // получение символа по индексу
    char& operator[](int index); // получение символа по индексу
    ~String(); // деструктор

    friend String operator*(int count, const String& string); // дружественная функция
    friend std::ostream& operator<<(std::ostream& os, const String& string); // вывод строки в поток
    friend std::istream& operator>>(std::istream& is, String& string); // ввод строки из потока

    static int GetStringsCount();
};

StringIndexErrorException::StringIndexErrorException() {
    std::cout << "StringIndexErrorException throwed" << std::endl;
}

StringOutOfMemoryException::StringOutOfMemoryException() {
    std::cout << "StringOutOfMemoryException throwed" << std::endl;
}

StringCountErrorException::StringCountErrorException() {
    std::cout << "StringCountErrorException throwed" << std::endl;
}

// конструктор по умолчанию
String::String() {
    size = 0; // строка пуста
    chars = nullptr; // пустая строка
    stringCount++;
}

// конструктор копирования
String::String(const String& string) {
    size = string.size; // копируем размер
    chars = new char[size]; // выделяем память

    if (chars == nullptr) // если не выделилась память
        throw StringOutOfMemoryException(); // бросаем исключение

    for (int i = 0; i < size; i++)
        chars[i] = string.chars[i]; // копируем символы

    stringCount++;
}

// конструктор из Си-строки
String::String(const char *s) {
    size = 0; // строка пуста
    chars = nullptr; // пустая строка

    for (int i = 0; s[i]; i++)
        Add(s[i]); // добавляем символ строки

    stringCount++;
}

// оператор присваивания
String& String::operator=(const String& string) {
    if (this == &string)
        return *this; // защищаем от самоприсваивания

    delete[] chars; // освобожаем память

    size = string.size; // копируем размер
    chars = new char[size]; // выделяем память

    if (chars == nullptr) // если не выделилась память
        throw StringOutOfMemoryException(); // бросаем исключение

    for (int i = 0; i < size; i++)
        chars[i] = string.chars[i]; // копируем символы

    return *this; // возвращаем объект
}

// получение длины
int String::GetLength() const {
    return size;
}

// пуста ли строка
bool String::IsEmpty() const {
    return size == 0;
}

// добавление символа в конец строки
void String::Add(char c) {
    char *tmp = new char[size + 1]; // создаём новый массив

    if (tmp == nullptr) // если не выделилась память
        throw StringOutOfMemoryException(); // бросаем исключение

    for (int i = 0; i < size; i++)
        tmp[i] = chars[i]; // копируем текущие символы

    delete[] chars; // освобождаем память
    chars = tmp; // обновляем укзаатель
    chars[size++] = c; // добавляем символ в массив
}

// добавление строки в конец строки
void String::Add(const String& string) {
    for (int i = 0; i < string.size; i++) {
        Add(string.chars[i]); // добавляем каждый символ строки
    }
}

// вставка символа по индексу
void String::Insert(char c, int index) {
    if (index < 0 || index > size) // если некорректный индекс
        throw StringIndexErrorException(); // бросаем исключение

    Add(c); // добавляем символ сначала в конец

    for (int i = size - 1; i > index; i--)
        chars[i] = chars[i - 1]; // сдвигаем символы вправо

    chars[index] = c; // вставляем символ на нужное место
}

// вставка строки по индексу
void String::Insert(const String& string, int index) {
    if (index < 0 || index > size) // если некорректный индекс
        throw StringIndexErrorException(); // бросаем исключение

    for (int i = 0; i < string.size; i++) {
        Insert(string.chars[i], index + i); // вставляем символы строки
    }
}

// удаление символа по индексу
void String::Remove(int index) {
    if (index < 0 || index >= size) // если некорректный индекс
        throw StringIndexErrorException(); // бросаем исключение

    size--; // уменьшаем размер

    for (int i = index; i < size; i++) {
        chars[i] = chars[i + 1]; // сдвигаем символы влево
    }
}

// удаление подстроки из интервала
void String::Remove(int start, int end) {
    if (start < 0 || start + end < 0 || start + end >= size) // если некорректный индекс
        throw StringIndexErrorException(); // бросаем исключение

    int length = end - start + 1; // получаем длину интервала
    size -= length; // уменьшаем размер на длину интервала

    for (int i = start; i < size; i++)
        chars[i] = chars[i + length]; // сдвигаем символы влево
}

// индекс вхождения подстроки
int String::IndexOf(const String& string) const {
    if (string.size > size) // если длина подстроки больше строки
        return -1; // такой строки точно нет

    // проверяем каждую подстроку на совпадение с искомой
    for (int i = 0; i <= size - string.size; i++) {
        int j = 0;

        while (j < string.size && chars[i + j] == string.chars[j])
            j++;

        if (j == string.size) // если дошли до конца искомой подстроки
            return i; // значит совпадают, возвращаем индекс
    }

    return -1; // не нашли, возвращаем -1
}

// замена подстроки на строку
void String::Replace(const String& substring, const String& replacement) {
    int index = IndexOf(substring); // ищем подстроку
    Remove(index, index + substring.size - 1); // удаляем подстроку
    Insert(replacement, index); // вставляем новую строку
}

// соединение двух строк
String String::operator+(const String& string) const {
    String result(*this); // копируем строку
    result.Add(string); // добавляем другую строку
    return result; // возвращаем результат
}

// умножение строки на число
String String::operator*(int count) const {
    if (count < 0) // если некорректное количество
        throw StringCountErrorException(); // бросаем исключение

    String result;

    for (int i = 0; i < count; i++)
        result.Add(*this); // добавляем строку

    return result;
}

// умножение строки на число
String operator*(int count, const String& string) {
    if (count < 0) // если некорректное количество
        throw StringCountErrorException(); // бросаем исключение

    String result;

    for (int i = 0; i < count; i++)
        result.Add(string); // добавляем строку

    return result;
}

// сравнение на равенство
bool String::operator==(const String& string) const {
    if (size != string.size) // если разные размеры
        return false; // то строки не равны

    for (int i = 0; i < size; i++)
        if (chars[i] != string.chars[i]) // если разные символы
            return false; // то не равны

    return true; // строки равны
}

// сравнение на меньше
bool String::operator<(const String& string) const {
    int i = 0;

    while (i < size && i < string.size) {
        if (chars[i] != string.chars[i]) // если разные символы
            return chars[i] < string.chars[i]; // то сравниваем их

        i++;
    }

    return i == size && i < string.size; // меньше, если кончилась первая строка
}

// сравнение на больше
bool String::operator>(const String& string) const {
    int i = 0;

    while (i < size && i < string.size) {
        if (chars[i] != string.chars[i]) // если разные символы
            return chars[i] > string.chars[i]; // то сравниваем их

        i++;
    }

    return i < size && i == string.size; // меньше, если кончилась первая строка
}

// сравнение на меньше или равно
bool String::operator<=(const String& string) const {
    return (*this < string) || (*this == string);
}

// сравнение на больше или равно
bool String::operator>=(const String& string) const {
    return (*this > string) || (*this == string);
}

// получение символа по индексу
char String::operator[](int index) const {
    if (index <= -size || index >= size) // если некорректный индекс
        throw StringIndexErrorException(); // бросаем исключение

    if (index < 0)
        return chars[size + index]; // аки в питоне :)

    return chars[index]; // возвраащем символ
}

// получение символа по индексу
char& String::operator[](int index) {
    if (index <= -size || index >= size) // если некорректный индекс
        throw StringIndexErrorException(); // бросаем исключение

    if (index < 0)
        return chars[size + index]; // аки в питоне :)

    return chars[index]; // возвраащем символ
}

// деструктор
String::~String() {
    delete[] chars; // освобождаем память
    stringCount--;
}

// вывод строки в поток
std::ostream& operator<<(std::ostream& os, const String& string) {
    for (int i = 0; i < string.size; i++)
        os << string.chars[i]; // выводим каждый символ

    return os;
}

// ввод строки из потока
std::istream& operator>>(std::istream& is, String& string) {
    string.size = 0; // обнуляем размер

    for (char c = is.get(); is && c != '\n'; c = is.get())
        string.Add(c); // добавляем символ

    return is; // возвращаем поток
}

int String::GetStringsCount() {
    return stringCount;
}

int String::stringCount = 0;
