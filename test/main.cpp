#include <chrono>
#include <iostream>
#include <SECS/secs.hpp>
#include <utl/utl.hpp>

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

std::ostream &operator<<(std::ostream &stream, Entity entity) {
    return stream << "[" << EntityPool::get_index(entity) << ":" << EntityPool::get_version(entity) << "]";
}

class TestEngine : public Engine<TestEngine> {};

int main() {

    Timer timer;

    TestEngine engine;
    Entity a, b, c;
    a = engine.create();
    b = engine.create();
    c = engine.create();

    engine.assign<int>(a, 15);
    engine.assign<std::string>(a, "a");
    engine.assign<std::string>(b, "b");
    print("Assigned to c:", engine.assign<std::string>(c, "c"));

    engine.assign<std::string>(b, "bb");

    for (auto [id, name] : engine.view<std::string>()) {
        print("Got entity:", id, name);
        name += name;
    }

    engine.assign<int>(c, 25);

    const TestEngine &const_engine = engine;

    REPEAT(3) {
        engine.destroy(b);
        b = engine.create();
    }

    engine.assign<int>(b);

    for (auto [id, name, counter] : const_engine.view<std::string, int>()) {
        print("Got entity:", id, name, counter);
    }

    for (auto id : const_engine.view()) {
        print("Got entity:", id);
    }

    return 0;
}
