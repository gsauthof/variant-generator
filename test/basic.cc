#include <boost/test/unit_test.hpp>

#include <basic_variant.hh>

#include <type_traits>

namespace {

  struct Visitor {
    typedef void result_type;
    int x {0};
    int y {0};
    template <typename T> void operator()(const T &t) { ++x; }
  };
  template <> void Visitor::operator()(const Foo &f) { ++y; }

  struct Collect_Value {
    typedef void result_type;
    int foo_i {0};
    int bar_i {0};
    int32_t i32 {0};
    std::string s;
    void operator()(const Foo &f)
    {
      foo_i = f.i;
    }
    void operator()(const Bar &f)
    {
      bar_i = f.i;
    }
    void operator()(int32_t x)
    {
      i32 = x;
    }
    void operator()(const std::string &x)
    {
      s = x;
    }
  };
  struct Update_String {
    typedef void result_type;
    template <typename T> void operator()(T &t) const { }
  };
  template <> void Update_String::operator()(std::string &s) const
  {
    s += "World";
  }

}

BOOST_AUTO_TEST_SUITE(variant)

  BOOST_AUTO_TEST_SUITE(basic)

      BOOST_AUTO_TEST_CASE(is_default_cons)
      {
        BOOST_CHECK_EQUAL(std::is_default_constructible<Basic_Variant>::value, true);
      }

      BOOST_AUTO_TEST_CASE(is_not_copy)
      {
        BOOST_CHECK_EQUAL(std::is_copy_constructible<Basic_Variant>::value, false);
      }

      BOOST_AUTO_TEST_CASE(destruct_on_scope_exit)
      {
        bool is_destructed = false;
        Visitor visitor;
        {
          Foo f(is_destructed);
          Basic_Variant v(std::move(f));
          v.accept(visitor);
        }
        BOOST_CHECK_EQUAL(visitor.x, 0);
        BOOST_CHECK_EQUAL(visitor.y, 1);
        BOOST_CHECK_EQUAL(is_destructed, true);
      }

      BOOST_AUTO_TEST_CASE(not_destruct_on_assign_same_tag)
      {
        bool is_destructed1 = false;
        bool is_destructed2 = false;
        Foo f(is_destructed1);
        Foo g(is_destructed2);
        {
          Basic_Variant v(std::move(f));
          v = std::move(g);
          BOOST_CHECK_EQUAL(is_destructed1, false);
        }
        BOOST_REQUIRE(f.is_destructed_ == nullptr);
        BOOST_REQUIRE(g.is_destructed_ == nullptr);
        BOOST_CHECK_EQUAL(is_destructed2, true);
      }

      BOOST_AUTO_TEST_CASE(destruct_on_assign)
      {
        bool is_destructed1 = false;
        bool is_destructed2 = false;
        Foo f(is_destructed1);
        Bar g(is_destructed2);
        {
          Basic_Variant v(std::move(f));
          v = std::move(g);
          BOOST_CHECK_EQUAL(is_destructed1, true);
        }
        BOOST_REQUIRE(f.is_destructed_ == nullptr);
        BOOST_REQUIRE(g.is_destructed_ == nullptr);
        BOOST_CHECK_EQUAL(is_destructed2, true);
      }

      BOOST_AUTO_TEST_CASE(move_variant)
      {
        bool is_destructed1 = false;
        bool is_destructed2 = false;
        Foo f(is_destructed1);
        f.i = 32;
        Foo g(is_destructed2);
        g.i = 42;
        {
          Basic_Variant v1(std::move(f));
          Basic_Variant v2(std::move(g));
          {
            Collect_Value c1;
            Collect_Value c2;
            v1.accept(c1);
            BOOST_CHECK_EQUAL(c1.foo_i, 32);
            v2.accept(c2);
            BOOST_CHECK_EQUAL(c2.foo_i, 42);
          }
          v1 = std::move(v2);
          BOOST_CHECK_EQUAL(is_destructed1, false);
          BOOST_CHECK_EQUAL(is_destructed2, false);
          Collect_Value c1;
          Collect_Value c2;
          v1.accept(c1);
          BOOST_CHECK_EQUAL(c1.foo_i, 42);
          v2.accept(c2);
          BOOST_CHECK_EQUAL(c2.foo_i, 0);
        }
        BOOST_CHECK_EQUAL(is_destructed1, false);
        BOOST_CHECK_EQUAL(is_destructed2, true);
      }

      BOOST_AUTO_TEST_CASE(move_variant_other)
      {
        bool is_destructed1 = false;
        bool is_destructed2 = false;
        Foo f(is_destructed1);
        f.i = 32;
        Bar g(is_destructed2);
        g.i = 42;
        {
          Basic_Variant v1(std::move(f));
          Basic_Variant v2(std::move(g));
          {
            Collect_Value c1;
            Collect_Value c2;
            v1.accept(c1);
            BOOST_CHECK_EQUAL(c1.foo_i, 32);
            v2.accept(c2);
            BOOST_CHECK_EQUAL(c2.bar_i, 42);
          }
          v1 = std::move(v2);
          BOOST_CHECK_EQUAL(is_destructed1, true);
          BOOST_CHECK_EQUAL(is_destructed2, false);
          Collect_Value c1;
          Collect_Value c2;
          v1.accept(c1);
          BOOST_CHECK_EQUAL(c1.bar_i, 42);
          v2.accept(c2);
          BOOST_CHECK_EQUAL(c2.bar_i, 0);
        }
        BOOST_CHECK_EQUAL(is_destructed1, true);
        BOOST_CHECK_EQUAL(is_destructed2, true);
      }

      BOOST_AUTO_TEST_CASE(throw_default)
      {
        Basic_Variant v;
        Collect_Value c;
        BOOST_CHECK_THROW(v.accept(c), std::domain_error);
      }

      BOOST_AUTO_TEST_CASE(throw_default_apply)
      {
        Basic_Variant v;
        Collect_Value c;
        BOOST_CHECK_THROW(v.accept(c), std::domain_error);
      }

      BOOST_AUTO_TEST_CASE(apply)
      {
        Basic_Variant v("Hello ");
        {
          Collect_Value c;
          v.accept(c);
          BOOST_CHECK_EQUAL(c.s, "Hello ");
        }
        v.apply(Update_String());
        Collect_Value c;
        v.accept(c);
        BOOST_CHECK_EQUAL(c.s, "Hello World");
      }

  BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
