#include <iostream>
#include "../include/parameter_pack.hpp"
#include "../include/component.hpp"
#include "../include/impl/entity.inl"
#include "../include/entity.hpp"
#include "../include/entity_routine.hpp"

template <typename ...Args>
void print(Args&& ...args) {
    ((std::cout << std::forward<Args>(args) << " "), ...);
    std::cout << std::endl;
}

class C1 : public Component {};

class C2 : public Component::Dependent<C1> {};

class C3 : public Component::Dependent<C2> {};

class C5;

class C4 : public Component::Dependent<C5> {};

class C5 : public Component::Dependent<C4> {};

class E1 : public Entity<C3, C4> {};

class E2 : public Entity<C5> {};

class E3 : public Entity<C1, C3, C5> {
    virtual void f() {};
};

int main(int argc, char *argv[]) {
    print(sizeof(EntityRoutine::Holder<E1>),
          sizeof(EntityRoutine::Holder<E2>),
          sizeof(EntityRoutine::Holder<E3>));
    print(sizeof(E1), sizeof(E2), sizeof(E3), sizeof(EntityRoutine::Metadata));
    // E3::BaseType e3;
    return 0;
}
