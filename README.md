C++ source code generator for [tagged unions][8].

The generated code uses C++11 features, most notable the C++11 `union`
construct. By default, move constructors and assignment operators are
generated for each of the included types. The value of a variant can
be changed/retrieved via function objects in a type-safe fashion.

Synonyms for tagged union are discriminated union, algebraic datatype
(cf.  Haskell) and variant, where variant seems to be popular among C++
libraries.

Georg Sauthoff <mail@georg.so>

## Example

Input specification (`variant.inf`):

    union {
      std::string
      uint32_t
      int32_t
    }

Generator call:

    $ mkvariant variant.inf

(which generates `variant.cc` and `variant.hh` by default)

Code can use the variant like this:

    struct Serialize_Visitor {
      typedef void result_type;
      Serializer &s;
      Serialize_Visitor(Serializer &s) : s(s) {}
      template <typename T> void operator()(const T &t) { s.write(t); }
    };

    void write_all(Serializer &s, const deque<Variant> &ds)
    {
      Serialize_Visitor sv(s);
      for (auto &d : ds)
        d.accept(sv);
    }

(assuming that `Serializer` has overloaded its method `write()` with
versions that do the right thing for each datatype)

Basically, a variant provides type dependent dispatch without the overhead
of inheritance (e.g. virtual functions and smart-pointers) and without ugly
switch-case statements distributed all over the code base.

## Design

- It is: a C++11 `union` wrapped by a class for tagging
- Code generation by a C++ program instead of a template meta programming
  approach (resulting in faster compile time)
- Move-constructors/assignment-operators are generated to assign/construct
  from one of the unionized types
- Copy-constructors/assignment-operators are not generated by default
  (can be enabled).
- Config can be specified in the [INFO][1] or JSON format
- Generation of the default constructor of the variant can be disabled. When
  the default constructor is available the variant can be empty. Trying to
  visit/apply an empty variant results in an exception.
- There are no getter/setter methods. Only visitation (via `apply()`/`visit()`).


## Related Work

[Boost Variant][2] is a class template that implements a tagged union. It does
not support move-construction/assignment from one of the unionized types.
The included value is accessed/modified via visitation or getters.
In general, it is a pre-C++11 design and it provides a 'never-empty' guarantee.
A policy-based revision is planned.

The [MMMLSTC][3] template library also includes a C++11/14 design of a variant.
Internally, [`std::aligned_storage()`][11] is used. The more obvious
[`std::aligned_union()`][12] (also in C++11) is not used, apparently to be
compatible to GCC 4.8/4.9 (GCC 5 has it). The variant template provides
move-construction/assignment from one of the unionized types.  It does not
provide copy-constructor/asignment operator for them. Besides a visitation
scheme it also includes a set of getters.

The CppCon 2014 talk ['Polymorphism with Unions'][5] describes a C++11 template
class (with selected elements from C++14/17) of a [variant][4] that internally
uses [`alignas()`][9]/[`alignof()`][10] and a function pointer table. It also
provides move-construction/assignment from one of the unionized types and no
copy-construction/assignment (variant objects itself can be copy-constructed,
though). A visitation scheme and getters is provided , as well. Unlike others,
it has helpers for using overloaded lambdas.

The [Facebook Template Library][6] (FATAL) contains a variant.
Internally, it uses a template class wrapper around a C++11 `union`. The
variant can be move-constructed from one of the unionized types, but an
'allocator' must be given as well. Move-assignment does not have this
restriction. It can be empty and it also can't be copy-constructed from one of
the unionized types.  Besides getters and a visitation scheme it also provides
setters.  The FATAL does not come with any documentation and the variant header
mentions that a 'major cleanup' is going on and that 'this implementation is
rather complex'.

[Qt also has a variant][7]. It basically includes all Qt-specific types. Custom
types with a default-constructor and copy-constructor can be added, as well.
The variant can also be empty. It only suppports copy-construction.  A
visitation scheme is not available, only a query/get scheme.

The oldest C++ related description of tagged unions is probably the [three][13]
part [article][14] series by [Alexandrescu][15] (Dr. Dobbs Journal, 2002).

## Compile

    $ mkdir build
    $ cd build
    $ cmake ..
    $ make

This creates the generator and also the unittests. Execute the unittests via:

    $ ./ut

## License

GPLv3+

[1]: http://www.boost.org/doc/libs/1_58_0/doc/html/property_tree/parsers.html#property_tree.parsers.info_parser
[2]: http://www.boost.org/doc/libs/1_58_0/doc/html/variant.html
[3]: https://github.com/mnmlstc/core
[4]: https://github.com/JasonL9000/cppcon14
[5]: https://www.youtube.com/watch?v=uii2AfiMA0o
[6]: https://github.com/facebook/fatal
[7]: http://doc.qt.io/qt-5/qmetatype.html
[8]: https://en.wikipedia.org/wiki/Tagged_union
[9]: http://en.cppreference.com/w/cpp/language/alignas
[10]: http://en.cppreference.com/w/cpp/language/alignof
[11]: http://en.cppreference.com/w/cpp/types/aligned_storage
[12]: http://en.cppreference.com/w/cpp/types/aligned_union
[13]: http://www.drdobbs.com/cpp/discriminated-unions-i/184403821
[14]: http://www.drdobbs.com/cpp/discriminated-unions-ii/184403828
[15]: http://www.drdobbs.com/generic-discriminated-unions-iii/184403834

