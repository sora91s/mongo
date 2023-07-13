#include <iostream>
#include "string"
void change2(std::string& basicString);
using namespace std;

// 引用传递
void change(std::string& str) {
    cout << "引用传递--函数操作地址" << &str << endl;
    str = "result";
    change2(str);
}

void change2(std::string& str) {
    str = "result2";
}

int main() {
    apple::CFUniquePtr<native_handle_type> _ssl;
    return 1;
}