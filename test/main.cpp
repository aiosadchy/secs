#include <iostream>

template <typename ...Args>
void print(Args&& ...args) {
    ((std::cout << args << " "), ...);
    std::cout << std::endl;
}


int main() {
    print("Hello", "world");
    return 0;
}
