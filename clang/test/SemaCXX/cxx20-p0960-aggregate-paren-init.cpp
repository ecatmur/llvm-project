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
// definitions for std::is_constructible
namespace std {
template<class T, class... A> constexpr bool is_constructible_impl(...) { return false; }
template<class T, class... A>
constexpr bool is_constructible_impl(decltype(new T(*(A*) nullptr...))) { return true; }
template<class T, class... A>
struct is_constructible { static constexpr bool value = is_constructible_impl<T, A...>(0); };
template<class T, class... A>
inline constexpr bool is_constructible_v = is_constructible<T, A...>::value;
}

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
// expected-error@-4{{excess elements in array initializer}}
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

struct E {
#if __cpp_aggregate_paren_init >= 201902
// expected-note@-2 +{{candidate constructor (the implicit copy constructor) not viable}}
// expected-note@-3 +{{candidate constructor (the implicit move constructor) not viable}}
#endif
  E(int);
#if __cpp_aggregate_paren_init >= 201902
// expected-note@-2 +{{candidate constructor not viable: requires 1 argument, but 0 were provided}}
#endif
};

E e1[3](1, 2, 3);
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{array initializer must be an initializer list}}
#endif

E e2[4](1, 2, 3);
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{array initializer must be an initializer list}}
#else
// expected-error@-4{{no matching constructor for initialization of 'E'}}
// expected-note@-5{{in implicit initialization of array element 3 with omitted initializer}}
#endif

struct A {
#if __cpp_aggregate_paren_init < 201902
// expected-note@-2 +{{candidate constructor (the implicit copy constructor) not viable}}
// expected-note@-3 +{{candidate constructor (the implicit move constructor) not viable}}
// expected-note@-4 +{{candidate constructor (the implicit default constructor) not viable}}
#endif
  int a;
  int&& r;
};

int f();
int n = 10;

A a1{1, f()};               // OK, lifetime is extended
A a2(1, f());               // well-formed, but dangling reference
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{no matching constructor for initialization of 'A'}}
#endif
A a3{1.0, 1};               // error: narrowing conversion
// expected-error@-1{{cannot be narrowed}}
// expected-note@-2{{insert an explicit cast to silence this issue}}
A a4(1.0, 1);               // well-formed, but dangling reference
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{no matching constructor for initialization of 'A'}}
#endif
A a5(1.0, std::move(n));    // OK
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{no matching constructor for initialization of 'A'}}
#endif

struct B {
#if __cpp_aggregate_paren_init < 201902
// expected-note@-2 +{{candidate constructor (the implicit copy constructor) not viable}}
// expected-note@-3 +{{candidate constructor (the implicit move constructor) not viable}}
// expected-note@-4 +{{candidate constructor (the implicit default constructor) not viable}}
#endif
  int a;
  int b;
};

B b1(n);
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{no matching constructor for initialization of 'B'}}
#endif
B b2(1.0);
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{no matching constructor for initialization of 'B'}}
#endif
B b3(1, 2, 3);
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{no matching constructor for initialization of 'B'}}
#else
// expected-error@-4{{excess elements in struct initializer}}
#endif

struct C {
#if __cpp_aggregate_paren_init < 201902
// expected-note@-2 +{{candidate constructor (the implicit copy constructor) not viable}}
// expected-note@-3 +{{candidate constructor (the implicit move constructor) not viable}}
// expected-note@-4 +{{candidate constructor (the implicit default constructor) not viable}}
#endif
  int i;
  E e;
#if __cpp_aggregate_paren_init >= 201902
// expected-note@-2{{in implicit initialization of field 'e' with omitted initializer}}
#endif
  E f = 10;
};
C c1(1);
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{no matching constructor for initialization of 'C'}}
#else
// expected-error@-4{{no matching constructor for initialization of 'E'}}
#endif
C c2(1, 2);
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{no matching constructor for initialization of 'C'}}
#endif

struct D : B, E {
#if __cpp_aggregate_paren_init < 201902
// expected-note@-2 +{{candidate constructor (the implicit copy constructor) not viable}}
// expected-note@-3 +{{candidate constructor (the implicit move constructor) not viable}}
// expected-note@-4 +{{candidate constructor (the implicit default constructor) not viable}}
#else
// expected-note@-9{{base class 'E' specified here}}
#endif
  int i;
};
D d1(B(1));
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{no matching conversion for functional-style cast from 'int' to 'B'}}
// expected-error@-3{{no matching constructor for initialization of 'D'}}
#else
// expected-error@-5{{no matching constructor for initialization of 'E'}}
#endif
D d2(B(), 10, 1);
#if __cpp_aggregate_paren_init < 201902
// expected-error@-2{{no matching constructor for initialization of 'D'}}
#endif

#ifndef __cpp_aggregate_paren_init
#   define __cpp_aggregate_paren_init 0
#endif
struct S { int i; int j; };
struct E { E(int); };
struct U { E e; int j; };
template<class T> struct V { T i = "nope"; };

static_assert(std::is_constructible_v<S>);
static_assert(std::is_constructible_v<S, int> == (__cpp_aggregate_paren_init >= 201902));
static_assert(std::is_constructible_v<S, long> == (__cpp_aggregate_paren_init >= 201902));
static_assert(std::is_constructible_v<S, float> == (__cpp_aggregate_paren_init >= 201902));
static_assert(std::is_constructible_v<S, int, int> == (__cpp_aggregate_paren_init >= 201902));
static_assert(not std::is_constructible_v<S, int, int, int>);
static_assert(not std::is_constructible_v<S, char*>);
static_assert(not std::is_constructible_v<S, int, char*>);
static_assert(not std::is_constructible_v<U>);
static_assert(std::is_constructible_v<U, int> == (__cpp_aggregate_paren_init >= 201902));
static_assert(std::is_constructible_v<U, int, int> == (__cpp_aggregate_paren_init >= 201902));

static_assert(std::is_constructible_v<int[2]>);
static_assert(std::is_constructible_v<int[2], int> == (__cpp_aggregate_paren_init >= 201902));
static_assert(std::is_constructible_v<int[2], int, int> == (__cpp_aggregate_paren_init >= 201902));
static_assert(not std::is_constructible_v<int[2], int, int, int>);
static_assert(std::is_constructible_v<int[], int, int> == (__cpp_aggregate_paren_init >= 201902));
static_assert(not std::is_constructible_v<E[2], int>);
static_assert(std::is_constructible_v<int[2], int, int> == (__cpp_aggregate_paren_init >= 201902));

static_assert(std::is_constructible_v<V<char const*>>);
static_assert(not std::is_constructible_v<V<int>>);
static_assert(std::is_constructible_v<V<char const*>, char const*> == (__cpp_aggregate_paren_init >= 201902));
static_assert(std::is_constructible_v<V<int>, int> == (__cpp_aggregate_paren_init >= 201902));
