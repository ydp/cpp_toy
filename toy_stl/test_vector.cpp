#include <vector>
#include <cassert>
#include <iostream>
#include <iterator>
#include <string>
//#include <array>

#include "x_stl/vector.h"

#include "test_util.h"

void test1()
{
    std::vector<std::string> v1(10, "ttt");
    X_STL::vector<std::string> v2(10, "ttt");
    assert(X_STL::Test::container_equal(v1, v2));
    X_STL::Test::print_container(v1, "std");
    X_STL::Test::print_container(v1, "xtd");
}



int main(int argc, char* argv[])
{
    test1();


    return 0;
}
