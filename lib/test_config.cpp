#include "config.h"

int main(int argc, char* argv[])
{
    Config config(argv[1]);
    config.print();
    cout << config.getConfig("port") << endl;
    cout << config.getInteger("port") << endl;
    return 0;
}
