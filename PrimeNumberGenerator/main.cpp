#include <iostream>
#include <cstdio>
#include <cstdlib>
#include <vector>
#include <algorithm>
#include <cmath>
#include <stack>
#include <functional>
#include <set>
#include <queue>
#include <string>
#include <map>
#include <iomanip>
#include <sstream>
#include <cassert>
#include "prime_gen.h"

int main() {
	int start, num;
	std::cin >> start >> num;
	PrimeNumberGenerator gen(start);
	for (int i = 0; i < num; ++i) {
		std::cout << gen.GetNextPrime() << " ";
	}
}