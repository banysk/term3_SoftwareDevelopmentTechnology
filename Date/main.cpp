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
#include "date.h"

int main() {
    {
        Date dt(1997, 1, 19);
        assert(!dt.IsLeap());
        assert(dt.ToString() == "19.01.1997");
    }
    {
        Date dt(100, 10, 1);
        assert(!dt.IsLeap());
        assert(dt.ToString() == "01.10.0100");
    }
    {
        Date dt(1, 1, 1);
        assert(!dt.IsLeap());
        assert(dt.ToString() == "01.01.0001");
    }
    {
        Date dt(2000, 10, 22);
        assert(dt.IsLeap());
        assert(dt.ToString() == "22.10.2000");
    }
    {
        Date dt(400, 1, 9);
        assert(dt.IsLeap());
        assert(dt.ToString() == "09.01.0400");
    }
    {
        Date dt(1996, 7, 8);
        assert(dt.IsLeap());
        assert(dt.ToString() == "08.07.1996");
    }
    {
        Date dt(2000, 1, 1);
        assert(dt.DaysLater(0).ToString() == "01.01.2000");
        assert(dt.DaysLater(10).ToString() == "11.01.2000");
        assert(dt.DaysLater(30).ToString() == "31.01.2000");
        assert(dt.DaysLater(31).ToString() == "01.02.2000");
        assert(dt.DaysLater(60).ToString() == "01.03.2000");
        assert(dt.DaysLater(366).ToString() == "01.01.2001");
    }
    {
        Date dt(2001, 1, 1);
        assert(dt.DaysLater(0).ToString() == "01.01.2001");
        assert(dt.DaysLater(10).ToString() == "11.01.2001");
        assert(dt.DaysLater(30).ToString() == "31.01.2001");
        assert(dt.DaysLater(31).ToString() == "01.02.2001");
        assert(dt.DaysLater(59).ToString() == "01.03.2001");
        assert(dt.DaysLater(365).ToString() == "01.01.2002");
    }
    {
        Date dt(2000, 1, 1);
        assert(dt.DaysLeft(Date{ 2000, 1, 1 }) == 0);
        assert(dt.DaysLeft(Date{ 2000, 1, 11 }) == 10);
        assert(dt.DaysLeft(Date{ 2000, 1, 31 }) == 30);
        assert(dt.DaysLeft(Date{ 2000, 2, 1 }) == 31);
        assert(dt.DaysLeft(Date{ 2000, 3, 1 }) == 60);
        assert(dt.DaysLeft(Date{ 2001, 1, 1 }) == 366);
    }
    {
        Date dt(2001, 1, 1);
        assert(dt.DaysLeft(Date{ 2001, 1, 1 }) == 0);
        assert(dt.DaysLeft(Date{ 2001, 1, 11 }) == 10);
        assert(dt.DaysLeft(Date{ 2001, 1, 31 }) == 30);
        assert(dt.DaysLeft(Date{ 2001, 2, 1 }) == 31);
        assert(dt.DaysLeft(Date{ 2001, 3, 1 }) == 59);
        assert(dt.DaysLeft(Date{ 2002, 1, 1 }) == 365);
    }
    std::cout << 1;
}