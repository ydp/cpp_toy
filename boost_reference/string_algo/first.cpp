#include <boost/algorithm/string.hpp>
#include <iostream>
using namespace std;
using namespace boost;


int main(int argc, char* argv[])
{
    string str1(" hello world! ");
    to_upper(str1);
    trim(str1);
    std::cout << str1 << std::endl;

    string str2 = to_lower_copy(ireplace_first_copy(str1, "hello", "goodbye"));
    std::cout << str2 << std::endl;
    return 0;
}
