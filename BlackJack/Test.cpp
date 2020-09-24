#include "Test.h"

Test::A::A(int i) {}
Test::B::B(int i) : A(i) {}
Test::C::C(int i) : A(i) {}