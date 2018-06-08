#include <iostream>
#include <string>
#include <algorithm>

using namespace std;

void perm(string s, int k, int m)
{
    if (k == m)
    {
        cout << s << endl;
    }
    else
    {
        for (int i = k; i < m; ++i)
        {
            swap(s[i], s[k]);
            perm(s, k+1, m);
            swap(s[i], s[k]);
        }
    }
}

int main(int argc, char* argv[])
{
    string s("abcd");
    perm(s, 0, s.size());

    return 0;
}
