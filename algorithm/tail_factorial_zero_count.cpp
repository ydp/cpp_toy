#include <iostream>

using namespace std;

long long count(long long n)
{
    if (n < 0)
    {
        return -1;
    }
    long long num = 0;
    while ((n/=5) > 0)
    {
        num += n;
    }
    return num;
}

int main(void)
{
    cout << "100:" << count(100) << endl;
    cout << "234100:" << count(234100) << endl;
    cout << "10230000:" << count(10230000) << endl;
    cout << "123:" << count(123) << endl;
    cout << "1000000000:" << count(1000000000) << endl;
    cout << "11123100:" << count(11123100) << endl;
    return 0;
}
