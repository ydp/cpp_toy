#include <iostream>
#include <vector>

using namespace std;

typedef int (*mapFunction)(int);
typedef int (*reduceFunction)(int,int);

#define ERROR -1

void map(mapFunction func, vector<int>& v)
{
    for (int i = 0; i < v.size(); ++i)
    {
        v[i] = func(v[i]);
    }
}

int reduce(reduceFunction func, vector<int>& v)
{
    if (v.size() == 0)
    {
        return ERROR;
    }
    int retVal = 0;
    for (int i = 0; i < v.size(); ++i)
    {
        retVal = func(retVal, v[i]);
    }
    return retVal;
}

int square(int i)
{
    return i * i;
}

int add(int i, int j)
{
    return i + j;
}

int main(void)
{
    vector<int> a(5);
    for (int i = 0; i < a.size(); ++i)
    {
        a[i] = i;
    }
    
    mapFunction mapFunctionPointer = (mapFunction)&square;
    reduceFunction reduceFunctionPointer = (reduceFunction)&add;

    map(mapFunctionPointer, a);
    int result = reduce(reduceFunctionPointer, a);

    cout << "The result is " << result << endl;
    return 0;
}
