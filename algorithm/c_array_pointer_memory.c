#include <stdio.h>

int main()
{
    int a[5] = {1,2,3,4,5};
    int *p1 = (int*)(&a + 1);
    int *p2 = (int*)((int)a + 1);
    //cout << p1[-1] << "," << *p2 << endl;
    printf("%x, %x\n", p1[-1], *p2);
    return 0;
}

