#include "bigint.h"

int main(int argc, char* argv[])
{
    std::ifstream in(argv[1]); // Read comma-separated operands
    string str;
    vector<string> v;
    getline(in, str, '\n');
    split(str,',',v); // Split dividend and divisor
    in.close();
    bigint a(v[1]), b(v[0]);
    div(a, b);
}
