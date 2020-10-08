#include <cassert>
#include <iostream>
#include <vector>
#include <limits>
#include <functional>

#include "num.h"

using namespace std;

void stress_test(int modulo, int st, int bound) {
    Num a(st, modulo);
    long long num = st;

    for (int i = 0; i < 100000; ++i) {
        int val = rand() % bound;

        if (i % 2) {
            a += val;
            num += val;
        }
        else {
            a *= val;
            num *= val;
        }
        num %= modulo;

        assert(static_cast<int>(num) == a.value);
    }
}

int main() {
    {
        Num a(1, 10);
        assert(a.value == 1);
        assert(a.modulo == 10);

        Num b(2, 10);
        assert(b.value == 2);
        assert(b.modulo == 10);

        Num c = a + b;
        assert(a.value == 1);
        assert(b.value == 2);
        assert(c.value == 3);
        assert(c.modulo == 10);

        c += a;
        assert(a.value == 1);
        assert(c.value == 4);
        assert(c.modulo == 10);

        c += 5;
        assert(c.value == 9);
        c += 2;
        assert(c.value == 1);
    }

    {
        Num a(1, 100);
        Num b(2, 100);
        Num c(3, 100);
        Num d = a + b + c;
        assert(d.value == 6);
        d += a + Num(4, 10) + 2;
        assert(d.value == 13);
        assert(d.modulo == 100);

        a = b = c;
        assert(a.value == 3);
        assert(b.value == 3);
        assert(c.value == 3);
    }

    {
        Num a(2, 100);
        assert(a.value == 2);
        assert(a.modulo == 100);

        Num b(3, 100);
        assert(b.value == 3);
        assert(b.modulo == 100);

        Num c = a * b;
        assert(a.value == 2);
        assert(b.value == 3);
        assert(c.value == 6);
        assert(c.modulo == 100);

        c *= a;
        assert(a.value == 2);
        assert(c.value == 12);
        assert(c.modulo == 100);

        c *= 2;
        assert(c.value == 24);
        c *= 10;
        assert(c.value == 40);
    }

    {
        Num a(1, 100);
        Num b(2, 100);
        Num c(3, 100);
        Num d = a * b * c;
        assert(d.value == 6);
        d *= a * Num(4, 10) * 2;
        assert(d.value == 48);
        assert(d.modulo == 100);
    }

    {
        Num a(3, 10);
        assert(a.value == 3);

        a += 11;
        assert(a.value == 4);
    }

    stress_test(137, 1, 10000);

    {
        Num a(1073741832, 1073741833);
        a += 2147483641;
        assert(a.value == 1073741807);

        Num b(1073741832, 1073741833);
        b *= 2147483641;
        assert(b.value == 25);
        assert(typeid(a.value) == typeid(int));
        assert(typeid(a.modulo) == typeid(int));
    }

    stress_test(1149499641, 1, numeric_limits<int>::max());

    {
        Num a(1, 10);
        assert(a.value == 1);
        assert(a.modulo == 10);

        Num b(2, 10);
        assert(b.value == 2);
        assert(b.modulo == 10);

        Num c = b - a;
        assert(a.value == 1);
        assert(b.value == 2);
        assert(c.value == 1);
        assert(c.modulo == 10);

        c -= 2;
        assert(c.value == 9);
        assert(c.modulo == 10);

        c += 5;
        assert(c.value == 4);
        c -= 4;
        assert(c.value == 0);
    }

    {
        Num a(10, 100);
        Num b(4, 100);
        Num c(2, 100);
        Num d = a - b - c;
        assert(d.value == 4);
        d -= a - Num(4, 10) - 2 + 8;
        assert(d.value == 92);
        assert(d.modulo == 100);
    }
    cout << 1 << endl;
}