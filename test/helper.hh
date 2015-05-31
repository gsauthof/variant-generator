#ifndef TEST_HELPER_HH
#define TEST_HELPER_HH


struct Foo {
  bool b{false};
  bool *is_destructed_{nullptr};
  int i {0};
  Foo(bool &is_destructed)
    :
      is_destructed_(&is_destructed)
  {
  }
  Foo(Foo &&o)
    :
      is_destructed_(o.is_destructed_),
      i(o.i)
  {
    o.is_destructed_ = nullptr;
    o.i = 0;
  }
  Foo &operator=(Foo &&o)
  {
    i = o.i;
    is_destructed_ = o.is_destructed_;
    o.is_destructed_ = nullptr;
    o.i = 0;
    return *this;
  }
  ~Foo()
  {
    if (is_destructed_)
      *is_destructed_ = true;
  }
};

struct Bar {
  bool b{false};
  bool *is_destructed_ {nullptr};
  int i {0};
  Bar(bool &is_destructed)
    :
      is_destructed_(&is_destructed)
  {
  }
  Bar(Bar &&o)
    :
      is_destructed_(o.is_destructed_),
      i(o.i)
  {
    o.is_destructed_ = nullptr;
    o.i = 0;
  }
  Bar &operator=(Bar &&o)
  {
    i = o.i;
    is_destructed_ = o.is_destructed_;
    o.is_destructed_ = nullptr;
    o.i = 0;
    return *this;
  }
  ~Bar()
  {
    if (is_destructed_)
      *is_destructed_ = true;
  }
};

#endif
