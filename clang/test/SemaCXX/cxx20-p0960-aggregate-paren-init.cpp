// RUN: %clang_cc1 -std=c++20 -verify %s
// RUN: %clang_cc1 -std=c++17 -verify %s

// p0960 Allow initializing aggregates from a parenthesized list of values
// p1975 Fixing the wording of parenthesized aggregate-initialization
// class.temporary/6
// dcl.init/17.5, 17.6

#if __cplusplus >= 202002 && __cpp_aggregate_paren_init < 201902
#   error "should set __cpp_aggregate_paren_init in C++20 mode"
#endif

// definitions for std::move
namespace std { template <class T> T &&move(T &); }

int i1[](1, 2, 3);
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{array initializer must be an initializer list}}
#else
static_assert(sizeof i1 == sizeof(int[3]));
#endif

int i2[2](1, 2, 3);
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{array initializer must be an initializer list}}
#else
// expected-error@-4{{too many initializers}}
#endif

int i3[4](1, 2, 3);
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{array initializer must be an initializer list}}
#endif

int i4[](1);
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{array initializer must be an initializer list}}
#else
static_assert(sizeof i4 == sizeof(int[1]));
#endif

struct A {
  int a;
  int&& r;
};
#if __cpp_aggregate_paren_init < 201902
// expected-note@-5{{candidate constructor (the implicit copy constructor) not viable}}
// expected-note@-6{{candidate constructor (the implicit move constructor) not viable}}
// expected-note@-7{{candidate constructor (the implicit default constructor) not viable}}
// expected-note@-8{{candidate constructor (the implicit copy constructor) not viable}}
// expected-note@-9{{candidate constructor (the implicit move constructor) not viable}}
// expected-note@-10{{candidate constructor (the implicit default constructor) not viable}}
// expected-note@-11{{candidate constructor (the implicit copy constructor) not viable}}
// expected-note@-12{{candidate constructor (the implicit move constructor) not viable}}
// expected-note@-13{{candidate constructor (the implicit default constructor) not viable}}
#endif

int f();
int n = 10;

A a1{1, f()};               // OK, lifetime is extended
A a2(1, f());               // well-formed, but dangling reference
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{no matching constructor}}
#endif
A a3{1.0, 1};               // error: narrowing conversion
// expected-error@-1{{cannot be narrowed}}
// expected-note@-2{{insert an explicit cast to silence this issue}}
A a4(1.0, 1);               // well-formed, but dangling reference
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{no matching constructor}}
#endif
A a5(1.0, std::move(n));    // OK
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{no matching constructor}}
#endif

struct B {
  int a;
  int b;
};
#if __cpp_aggregate_paren_init < 201902
// expected-note@-5{{candidate constructor (the implicit copy constructor) not viable}}
// expected-note@-6{{candidate constructor (the implicit move constructor) not viable}}
// expected-note@-7{{candidate constructor (the implicit default constructor) not viable}}
// expected-note@-8{{candidate constructor (the implicit copy constructor) not viable}}
// expected-note@-9{{candidate constructor (the implicit move constructor) not viable}}
// expected-note@-10{{candidate constructor (the implicit default constructor) not viable}}
// expected-note@-11{{candidate constructor (the implicit copy constructor) not viable}}
// expected-note@-12{{candidate constructor (the implicit move constructor) not viable}}
// expected-note@-13{{candidate constructor (the implicit default constructor) not viable}}
#endif

B b1(n);
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{no matching constructor}}
#endif
B b2(1.0);
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{no matching constructor}}
#endif
