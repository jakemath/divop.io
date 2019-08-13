#include "bigint.h"

int main(int argc, char *argv[])
{
    bigint a(atoi(argv[1]),1); // Construct rule for valid random number
    short ones[4] = {7, 9, 3, 1};
    std::random_device rd;
    if (a.get_data().front() % 2 == 0 || a.get_data().front() == 5)
    {
        if (a.get_data().size() == 1)
            a.get_data().front() = ones[rd() % 2];
        else
            a.get_data().front() = ones[rd() % 4];
    }
    else if (a.get_data().front() == 1 && a.get_data().size() == 1)
        a.get_data().front() = ones[rd() % 2];
    cout << a;
}
