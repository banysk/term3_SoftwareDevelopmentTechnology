#pragma once
#include "num.h"

Num::Num(int value, int modulo ) {
    if (modulo) {
        this->value = value % modulo;
        this->modulo = modulo;
    } else {
        this->value = value;
        this->modulo = 0;
    }
}
Num::Num(const Num& other) {
    this->value = other.value;
    this->modulo = 0;
}
