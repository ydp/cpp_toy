#include <iostream>

using namespace std;

long count(long n)
{
    if (n < 0)
    {
        return -1;
    }
    long num = 0;
    long tail = 1;
    while (true)
    {
        tail = tail * 10; 
        if (n % tail == 0)
        {
            num += 1;
        }else 
        {
            break;
        }
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
