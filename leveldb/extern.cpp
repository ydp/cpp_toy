#include "extern.h"

namespace level{
    int add (int a, int b)
    {
        return a + b;
    }

}

int main(int argc, char* argv[])
{
    using namespace level;
    
    std::cout << add(2, 3) << std::endl;

    return 0;
}
