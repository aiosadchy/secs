#include <chrono>
#include <iostream>
#include "../include/collections/sparse_map.hpp"

template <typename ...Args>
void print(Args&& ...args) {
    ((std::cout << args << " "), ...);
    std::cout << std::endl;
}

class Timer {
public:
    Timer() : m_start() {
        restart();
    }

    double restart() {
        auto now = std::chrono::high_resolution_clock::now();
        auto before = m_start;
        m_start = now;
        auto dur = now - before;
        auto mcs = std::chrono::duration_cast<std::chrono::microseconds>(dur)
                .count();
        return seconds(mcs);
    }

    double get_time() const {
        auto now = std::chrono::high_resolution_clock::now();
        auto dur = now - m_start;
        auto mcs = std::chrono::duration_cast<std::chrono::microseconds>(dur)
                .count();
        return seconds(mcs);
    }

    static double seconds(long long microseconds) {
        return static_cast<double>(microseconds) * 1e-6;
    }

private:
    std::chrono::high_resolution_clock::time_point m_start;

};

template <typename T>
struct t {
    int i;
    T other;
};

struct C {
    alignas(1) char data[17];

    C(char value = '=') {
        for (auto &c : data) {
            c = value;
        }
    }

    char value() const {
        return data[11];
    }
};

int main() {
    SparseMap<C> map(16);

    Timer timer;

    print(map.contains(125));

    REPEAT(5) {
        map.put(rand() % 100, 'a' + rand() % 12);
    }

    print(map[220].value());

    map.remove(220);

    for (Size i : map.get_keys()) {
        print(i);
    }

    print();

    for (auto &c : map.get_values()) {
        print(c.value());
    }


    return 0;
}
