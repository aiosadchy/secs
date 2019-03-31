#include <iostream>
#include "../include/parameter_pack.hpp"
#include "../include/component.hpp"
#include "../include/impl/entity.inl"
#include "../include/entity.hpp"

void testObjectPool();

template <typename T>
void debug(T&& t, const char *str = "") {
    std::cout << str << t << std::endl;
}

class C1 : public Component {};

class C2 : public Component::Dependent<C1> {};

class C3 : public Component::Dependent<C2> {};

class C5;

class C4 : public Component::Dependent<C5> {};

class C5 : public Component::Dependent<C4> {};

class E1 : public Entity<C3, C4> {};

class E2 : public Entity<C5> {};

class E3 : public Entity<C1, C3, C5> {};

int main(int argc, char *argv[]) {
    testObjectPool();
    E1 e1;
    E2 e2;
    E3 e3;
    return 0;
}
