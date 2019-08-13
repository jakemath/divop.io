#include "bigint.h"

int main(int argc, char *argv[])
{
    std::ifstream in(argv[1]); // Read first operand in file
    string str;
    getline(in,str,',');
    in.close();
    bigint a(str), rule; // Compute rule for operand
    if (a.get_data().front() == 1 || a.get_data().front() == 9)
    rule = a;
    else
    rule = a * 3;
    if (rule.get_data().front() == 1)
    {
        rule.get_data().pop_front();
        rule.get_data().back() *= -1;
    }
    else
    {
        if (rule.get_data().size() > 1)
        rule.get_data().pop_front();
        ++rule;
    }
    cout << rule;
}
