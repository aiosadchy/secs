#include <algorithm>
#include <random>
#include <iostream>
#include "../include/object_pool.hpp"

#define TEST_OBJECT_COUNT 100000
#define TEST_OBJECT_SIZE 16

struct TestObject {
    explicit TestObject(char c) : data() {
        for (auto &d : data) {
            d = c;
        }
        ++count;
    }

    ~TestObject() {
        --count;
    }

    bool check(char c) {
        for (auto &d : data) {
            if (d != c) {
                return false;
            }
        }
        return true;
    }

    char data[TEST_OBJECT_SIZE];
    static int count;
};

int TestObject::count = 0;

TestObject *objects[TEST_OBJECT_COUNT];
unsigned eraseorder[TEST_OBJECT_COUNT];

void testObjectPool() {
    std::cout << "TEST STARTED: Object Pool" << std::endl;

    ObjectPool<TestObject> pool(16);

    for (unsigned i = 0; i < TEST_OBJECT_COUNT; ++i) {
        eraseorder[i] = i;
    }
    std::shuffle(std::begin(eraseorder), std::end(eraseorder), std::mt19937(std::random_device()()));

    for (unsigned i = 0; i < TEST_OBJECT_COUNT; ++i) {
        objects[i] = pool.create(static_cast<char>(i % 256));
    }

    std::cout << TestObject::count << " of " << TEST_OBJECT_COUNT << " objects created" << std::endl;

    for (auto i : eraseorder) {
        if (!objects[i]->check(static_cast<char>(i % 256))) {
            std::cout << i << std::endl;
        }
    }

    for (auto i : eraseorder) {
        pool.destroy(objects[i]);
    }

    std::cout << TestObject::count << " objects left (expected: " << 0 << ")" << std::endl;

    std::cout << "TEST FINISHED: Object Pool" << std::endl;
}