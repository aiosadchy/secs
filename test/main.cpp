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

std::ostream &operator<<(std::ostream &stream, const Entity::ID &entity) {
    return stream << "[" << entity.get_index() << ":" << entity.get_version() << "]";
}

class TestEngine : public Engine<TestEngine> {};

int main() {

    Timer timer;

    TestEngine engine;
    Entity::ID a, b, c;
    a = engine.create();
    b = engine.create();
    c = engine.create();

    engine.assign<int>(a, 15);
    engine.assign<std::string>(a, "a");
    engine.assign<std::string>(b, "b");
    print("Assigned to c:", engine.assign<std::string>(c, "c"));

    engine.assign<std::string>(b, "bb");

    for (auto [id, name] : engine.iterate<std::string>()) {
        print("Got entity:", id.get_index(), name);
        name += name;
    }

    engine.assign<int>(c, 25);

    const TestEngine &const_engine = engine;

    REPEAT(3) {
        engine.destroy(b);
        b = engine.create();
    }

    for (auto [id, name, counter] : const_engine.iterate<std::string, int>()) {
        print("Got entity:", id.get_index(), name, counter);
    }

    for (auto id : const_engine.iterate()) {
        print("Got entity:", id.get_index());
    }

    return 0;
}
