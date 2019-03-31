#include <iostream>

void testObjectPool();

template <typename T>
void debug(T&& t, const char *str = "") {
    std::cout << str << t << std::endl;
}

int main(int argc, char *argv[]) {
    testObjectPool();
    return 0;
}
