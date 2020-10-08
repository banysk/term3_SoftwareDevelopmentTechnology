#pragma once

class Num {
public:
    Num(int value, int modulo);

    Num& operator=(const Num& other);

    Num operator+(const Num& other);
    Num operator-(const Num& other);
    Num operator*(const Num& other);
    Num operator+(int num);
    Num operator-(int num);
    Num operator*(int num);

    Num& operator+=(const Num& other);
    Num& operator-=(const Num& other);
    Num& operator*=(const Num& other);
    Num& operator+=(int num);
    Num& operator-=(int num);
    Num& operator*=(int num);

    int value;
    int modulo;
};