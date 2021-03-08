#include <chrono>
#include <iostream>
#include <SECS/collections/sparse_map.hpp>
#include <SECS/component.hpp>

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
        using namespace std::chrono;
        auto now = high_resolution_clock::now();
        auto before = m_start;
        m_start = now;
        auto dur = now - before;
        auto mcs = duration_cast<std::chrono::microseconds>(dur).count();
        return seconds(mcs);
    }

    double get_time() const {
        using namespace std::chrono;
        auto now = high_resolution_clock::now();
        auto dur = now - m_start;
        auto mcs = duration_cast<std::chrono::microseconds>(dur).count();
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
    char data[17];

    C(char value) {
        for (auto &c : data) {
            c = value;
        }
    }

    ~C() {
    }

    friend std::ostream &operator<<(std::ostream &os, const C &c) {
        os << c.data[11];
        return os;
    }
};

int main() {
    using Component = ::Component<void>;
    Component::Metadata metadata = Component::Metadata::get<int>();


    SparseMap<C> map(16);

    Timer timer;

    UTL_REPEAT(1000) {
        map.put(rand() % 100, 'a' + rand() % 23);
        if (rand() % 2 == 0) {
            map.remove(rand() % 10);
        }
    }

    UTL_REPEAT(5) {
        int key = rand() % 100;
        if (map.contains(key)) {
            print(key, map.get(key));
        } else {
            print(key, "no value");
        }
    }

    return 0;
}
