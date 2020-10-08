#pragma once
#include <functional>
#include "num.h"

Num::Num(int value = 0, int modulo = 0) {
    this->value = value;
    this->modulo = modulo;
    if (modulo) {
        this->value %= modulo;
    }
}

Num& Num::operator=(const Num& other) {
    this->value = other.value;
    this->modulo = other.modulo;
    return *this;
}

Num Num::operator+(const Num& other) {
    int64_t num = this->value % modulo;
    num = (num + (other.value % modulo)) % modulo;
    if (num < 0) {
        num += modulo;
    }
    return Num(num, modulo);
}

Num Num::operator-(const Num& other) {
    int64_t num = this->value % modulo;
    num = (num - (other.value % modulo)) % modulo;
    if (num < 0) {
        num += modulo;
    }
    return Num(num, modulo);
}

Num Num::operator*(const Num& other) {
    int64_t num = this->value % modulo;
    num = (num * (other.value % modulo)) % modulo;
    if (num < 0) {
        num += modulo;
    }
    return Num(num, modulo);
}

Num Num::operator+(int other) {
    int64_t num = this->value % modulo;
    num = (num + (other % modulo)) % modulo;
    if (num < 0) {
        num += modulo;
    }
    return Num(num, modulo);
}

Num Num::operator-(int other) {
    int64_t num = this->value % modulo;
    num = (num - (other % modulo)) % modulo;
    if (num < 0) {
        num += modulo;
    }
    return Num(num, modulo);
}

Num Num::operator*(int other) {
    int64_t num = this->value % modulo;
    num = (num * (other % modulo)) % modulo;
    if (num < 0) {
        num += modulo;
    }
    return Num(num, modulo);
}

Num& Num::operator+=(const Num& other) {
    int64_t num = this->value % modulo;
    this->value = num = (num + (other.value % modulo)) % modulo;
    if (num < 0) {
        this->value = num + modulo;
    }
    return *this;
}

Num& Num::operator-=(const Num& other) {
    int64_t num = this->value % modulo;
    this->value = num = (num - (other.value % modulo)) % modulo;
    if (num < 0) {
        this->value = num + modulo;
    }
    return *this;
}

Num& Num::operator*=(const Num& other) {
    int64_t num = this->value % modulo;
    this->value = num = (num * (other.value % modulo)) % modulo;
    if (num < 0) {
        this->value = num + modulo;
    }
    return *this;
}

Num& Num::operator+=(int other) {
    int64_t num = this->value % modulo;
    this->value = num = (num + (other % modulo)) % modulo;
    if (num < 0) {
        this->value = num + modulo;
    }
    return *this;
}

Num& Num::operator-=(int other) {
    int64_t num = this->value % modulo;
    this->value = num = (num - (other % modulo)) % modulo;
    if (num < 0) {
        this->value = num + modulo;
    }
    return *this;
}

Num& Num::operator*=(int other) {
    int64_t num = this->value % modulo;
    this->value = num = (num * (other % modulo)) % modulo;
    if (num < 0) {
        this->value = num + modulo;
    }
    return *this;
}
