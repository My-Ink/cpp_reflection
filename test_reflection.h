#ifndef TEST_REFLECTION_H_
#define TEST_REFLECTION_H_

#include <iostream>
#include <cassert>
#include "reflect.h"

class X {
    // Private constructor
    X() {}
 public:
    int v = 9;
};

void test() {
    my::reflect::Instance<X> ix;

    // You can use this syntax for access to public fields
    (*ix).v = 111;
    assert((*ix).v == 111);

    // You can get special class-object
    my::reflect::Class<X> x = ix.get_class();

    assert(ix.is_instance_of(x));

    int X::*p_to_m = &X::v;

    // You also can use this functions for access to public fields
    ix = x.get_instance();
    ix.set_field(p_to_m, 2);
    assert(ix.get_field(p_to_m) == 2);
}

#endif //TEST_REFLECTION_H_
