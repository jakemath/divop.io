#include "bigint.h"

int main(int argc, char* argv[])
{
    std::ifstream in(argv[1]);
    string str;
    vector<string> v;
    while (!in.eof())
    {
        getline(in, str, '\n');
        split(str,',',v);
    }
    in.close();
    bigint a(v[1]), b(v[0]);
    div(a, b);
}
