#include <iostream>
#include <string>

using std::cout;
using std::cin;
using std::endl;
using std::string;


// 文件结束符 mac: Ctrl+d

int main()
{
    string line;
    while (getline(cin, line))
    {
        cout << line << endl;
    }

    cout << "end" << endl;
    return 0;
}
