#include <iostream>

using namespace std;

int partition(int n, int m)
{
    if (m == 1 || n == 1)
    {
        return 1;
    }else if (m > n)
    {
        return partition(n, n);
    }else if (m == n)
    {
        return 1 + partition(n, n-1);
    }else
    /// 6,5 -> 6,4   1,5
    /// 6,4 -> 6,3   2,4
    /// 6,3 -> 6,2   3,3
    /// 6,2 -> 6,1   4,2
    {
        return partition(n, m-1) + partition(n-m, m);
    }
}


int main(int argc, char * argv[])
{
    cout << partition(6, 6) << endl;

    return 0;
}
