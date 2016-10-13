#include "jjalloc.h"
#include "jjvector.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
	int ia[5] = {0, 1, 2, 3, 4};
	unsigned int i;

	JJ::JJVector<int> iv(ia, ia+5);
	for (i = 0; i < iv.size(); ++i)
	{
		cout << iv[i] << ' ';
	}
	cout << endl;

	return 0;
}
