#include <boost/test/unit_test.hpp>

#include <copy.hh>

#include <type_traits>

using namespace std;

namespace {

  struct Get_String {
    typedef void result_type;
    string s;

    template <typename T> void operator()(const T &t) {}
  };
  template <> void Get_String::operator()(const string &x)
  {
    s = x;
  }

}

BOOST_AUTO_TEST_SUITE(variant)

  BOOST_AUTO_TEST_SUITE(copy)

      BOOST_AUTO_TEST_CASE(is_default_cons)
      {
        BOOST_CHECK_EQUAL(std::is_default_constructible<Variant>::value, true);
      }

      BOOST_AUTO_TEST_CASE(is_copy)
      {
        BOOST_CHECK_EQUAL(std::is_copy_constructible<Variant>::value, true);
      }

      BOOST_AUTO_TEST_CASE(copy_value)
      {
        const string cs("Hello World");
        Variant v(cs);
        {
          Get_String c;
          v.visit(c);
          BOOST_CHECK_EQUAL(c.s, "Hello World");
        }
        const string ds("foo bar");
        v = ds;
        {
          Get_String c;
          v.visit(c);
          BOOST_CHECK_EQUAL(c.s, "foo bar");
        }
      }

      BOOST_AUTO_TEST_CASE(copy_variant)
      {
        Variant v("hello");
        Variant w("world");
        v = w;
        {
          Get_String c;
          v.visit(c);
          BOOST_CHECK_EQUAL(c.s, "world");
        }
        {
          Get_String c;
          w.visit(c);
          BOOST_CHECK_EQUAL(c.s, "world");
        }
      }


  BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
