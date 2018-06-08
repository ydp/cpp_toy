#include <iostream>


int main()
{
    // << 操作符接收两个对象，返回左侧对象
    // std::endl 结束当前行，并将与设备关联的缓冲区内容刷新到设备中
    std::cout << "Enter two numbers:" << std::endl;
    int v1 = 0, v2 = 0;
    std::cin >> v1 >> v2;
    std::cout << "The sum of " << v1 << " and " << v2
        << " is " << v1 + v2 << std::endl;
    return 1;
}
