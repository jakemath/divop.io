#include "bigint.h"

int main(int argc, char *argv[])
{
    bigint a(atoi(argv[1]), 1), b(atoi(argv[2]), 1); // Construct valid operands for div
    short ones[4] = {3,7,9,1};
    std::random_device rd;
    if (b.get_data().size() == 1)
    {
        if (b.get_data().front() != 2 && b.get_data().front() % 2 == 0)
            b.get_data().front() = ones[rd() % 3];
    }
    else if (b.get_data().front() % 2 == 0 || b.get_data().front() == 5)
        b.get_data().front() = ones[rd() % 4];
    cout << b << "," << a;
}
