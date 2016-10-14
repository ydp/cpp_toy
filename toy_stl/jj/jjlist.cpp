#include "jjlist.h"
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{

	JJ::JJList<int> iv(5, 2);
	iv.print();

	return 0;
}
