#pragma once

#include <iostream>

class Num {
public:
    Num(int value=0, int modulo=0);
    Num(const Num& other);
    
    int value;
    int modulo;
};