#include <boost/test/unit_test.hpp>

#include <json.hh>

#include <type_traits>

namespace {

  struct Collect_Value {
    typedef void result_type;
    int32_t i32 {0};
    std::string s;
    void operator()(int32_t x);
    void operator()(const std::string &x);
  };

  void Collect_Value::operator()(int32_t x)
  {
    i32 = x;
  }
  void Collect_Value::operator()(const std::string &x)
  {
    s = x;
  }

}

BOOST_AUTO_TEST_SUITE(variant)

  BOOST_AUTO_TEST_SUITE(ns)

      BOOST_AUTO_TEST_CASE(is_default_cons)
      {
        BOOST_CHECK_EQUAL(std::is_default_constructible<Foo::Bar::Json>::value, true);
      }

      BOOST_AUTO_TEST_CASE(basic)
      {
        Foo::Bar::Json v(32);
        Collect_Value c;
        BOOST_CHECK_EQUAL(c.i32, 0);
        v.visit(c);
        BOOST_CHECK_EQUAL(c.i32, 32);
      }

  BOOST_AUTO_TEST_SUITE_END()

BOOST_AUTO_TEST_SUITE_END()
