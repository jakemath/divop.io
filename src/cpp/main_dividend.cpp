/*
Author: Jake Mathai
Purpose: Dividend main file - runs a series of trials with varying dividend sizes for runtime analysis on F(m)
*/

#include "bigint.h"

int main (int argc, char* argv[]) {
    unsigned long long dividend_upper = atoi(argv[1]), dividend_size;
//    bigint b(10, true);
//    short odds[4] = {7,9,3,1};
//    if (b.digits.front() % 2 == 0) {
//        srand(time(0));
//        if (b.digits.size() > 1)
//            b.digits().front() = odds[rand() % 4];
//        else if (b.digits.size() == 1 && b.digits.front() != 2)
//            b.digits().front() = odds[rand() % 3];
//    }
//    if (b.digits.front() == 5 && b.digits.size() > 1)
//        b.digits().front() = odds[rand() % 4];
//    else if (b.digits.front() == 1 && b.digits.size() == 1)
//        b.digits().front() = odds[rand() % 3];
    for (unsigned long long n = dividend_upper, m = 1; n >= int(dividend_upper/2) && m < int(dividend_upper)/2; --n, ++m) {
        Bigint a(n - m, true), b(m, true);
        short odds[4] = {7,9,3,1};
        if (b.digits.front() % 2 == 0) {
            srand(time(0));
            if (b.digits.size() > 1)
                b.digits.front() = odds[rand() % 4];
            else if (b.digits.size() == 1 && b.digits.front() != 2)
                b.digits.front() = odds[rand() % 3];
        }
        if (b.digits.front() == 5 && b.digits.size() > 1)
            b.digits.front() = odds[rand() % 4];
        else if (b.digits.front() == 1 && b.digits.size() == 1)
            b.digits.front() = odds[rand() % 3];
        a = a * b;
        dividend_size = a.digits.size();
        bool divisible;
        auto start = std::chrono::steady_clock::now();
        divisible = div(a,b);
        auto end = std::chrono::steady_clock::now();
        std::cout << divisible << " " << dividend_size << " " << b.digits.size() << " " 
            << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << std::endl;
    }
}
