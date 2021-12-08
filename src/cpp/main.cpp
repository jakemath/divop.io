/*
Author: Jake Mathai
Purpose: Bigint main file. CLI args are as follows:
    $1 - Dividend size in digits
    $2 - Divisor size in digits
Program first generates a divisor at the specified size then generates a divisor of approximately the specified size
by multiplying the divisor by a randomly generated temporary operand to yield an appropriately sized dividend operand.
This implies the return value should always be true for this program
*/

#include "bigint.h"

int main(int argc, char* argv[]) {
    unsigned long long dividend_size = atoi(argv[1]), divisor_size = atoi(argv[2]);
    std::cout << "Generating operands..." << std::endl;
    if (dividend_size < divisor_size) {
        std::cout << "Dividend size must be >= divisor size" << std::endl;
        exit(1);
    }
    else if (dividend_size == divisor_size)
        dividend_size = 1;
    else
        dividend_size -= divisor_size;
    Bigint a(dividend_size, 1), b;
    short odds[4] = {7,9,3,1};
    if (divisor_size == 1) {
        srand(time(0));
        b.digits.push_back(odds[rand() % 2]);
    }
    else
        b = Bigint(divisor_size, 1);
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
    std::cout << "Computing divisibility..." << std::endl;
    bool divisible;
    auto start = std::chrono::steady_clock::now();
    divisible = div(a,b);
    auto end = std::chrono::steady_clock::now();
    std::cout << "A | B = " << (divisible ? "True" : "False") << "\nDividend size = " << dividend_size
         << "\nDivisor size = " << b.digits.size() << "\nRuntime = "
         << std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count() << "ms" << std::endl;
}
